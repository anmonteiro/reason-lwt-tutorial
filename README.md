reason-lwt-tutorial
---

This small project implements the solutions of the Mirage [Lwt tutorial](https://mirage.io/wiki/tutorial-lwt)
in Reason, using [bsb-native](https://github.com/bsansouci/bsb-native) for compilation.

## Install

```shell
npm i
```

## Build

```shell
npm run build
```

### Building to native

The above command will compile to bytecode by default. If you want to compile
to native, run the following command:

```shell
npm run build -- -backend native
```

## Run
```shell
./lib/bs/bytecode/index.byte
```

### Running the native build

``` shell
./lib/bs/native/index.native
```

## License

MIT. See [LICENSE](./LICENSE).
