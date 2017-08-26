open Lwt.Infix;

/* Challenge 1: Sleep and Join */
module SleepJoin = {
  let compose f g x => f (g x);
  let minToMs = compose float_of_int (( * ) 1000);
  let start () =>
    Lwt.join [
      Lwt_unix.sleep 1.0 >>= (fun () => Lwt_io.write Lwt_io.stdout "Heads\n"),
      Lwt_unix.sleep 2.0 >>= (fun () => Lwt_io.write Lwt_io.stdout "Tails\n")
    ]
    >>= (fun () => Lwt_io.write Lwt_io.stdout "Finished.\n");
};

let () = Lwt_main.run @@ SleepJoin.start ();

/*
 * Challenge 2: Looping echo server - we implement actual reading from stdin
 * because it's more fun than generating random strings
 */
module EchoServer = {
  let read_line () =>
    Lwt_io.write Lwt_io.stdout "Echo prompt=> "
    >>= (fun () => Lwt_io.read_line Lwt_io.stdin);
  let start () => {
    let rec echo_server =
      fun
      | 0 => Lwt.return ()
      | n =>
        read_line ()
        >>= (fun line => Lwt_io.write Lwt_io.stdout (line ^ "\n"))
        >>= (fun () => echo_server (n - 1));
    echo_server 10
  };
};

let () = Lwt_main.run @@ EchoServer.start ();

/* Challenge 3: Timeouts */
module Timeouts = {
  let timeout delay t =>
    Lwt_unix.sleep delay
    >>= (
      fun () =>
        Lwt.(
          switch (Lwt.state t) {
          | Sleep =>
            Lwt.cancel t;
            Lwt.return None
          | Return x => Lwt.return (Some x)
          | Fail exn => Lwt.fail exn
          }
        )
    );
  let start () => {
    let t = Lwt_unix.sleep (Random.float 3.0) >|= (fun () => "Heads");
    timeout 2.0 t
    >>= (
      fun
      | None => Lwt_io.write Lwt_io.stdout "Cancelled"
      | Some v => Lwt_io.write Lwt_io.stdout (Printf.sprintf "Returned %S" v)
    )
  };
};

let () = {
  Random.self_init ();
  Lwt_main.run @@ Timeouts.start ()
};

/* Challenge 4: Better timeouts */
module BetterTimeouts = {
  let timeout delay t => {
    let tmout = Lwt_unix.sleep delay;
    Lwt.pick [tmout >|= (fun () => None), t >|= (fun v => Some v)]
  };
  let start () => {
    let t = Lwt_unix.sleep (Random.float 3.0) >|= (fun () => "Heads");
    timeout 2.0 t
    >>= (
      fun
      | None => Lwt_io.write Lwt_io.stdout "Cancelled"
      | Some v => Lwt_io.write Lwt_io.stdout (Printf.sprintf "Returned %S" v)
    )
  };
};

let () = {
  Random.self_init ();
  Lwt_main.run @@ BetterTimeouts.start ()
};
