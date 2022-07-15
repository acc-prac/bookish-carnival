# Building with Singularity & CMake 

## Debug Build

For configuring a debug build of the project and building tests, executing inside the 
given acc-prac container seems to be necessary to discover the path to the `nvc++` compiler.

Therefore, to build tests, execute:
```sh
sh singularity/dbuild.sh
```

To execute said tests, as access to a device may be necessary, the tests can be queued with
```sh
sh singularity/tests.sh
```


## Release Build

For configuring a release build of the project and building tests, again, executing inside the 
given acc-prac container seems to be necessary.

Therefore, to build the benchmarks, execute:
```sh
sh singularity/rbuild.sh
```


To execute said benchmarks in a simple manner, queue them with
```sh
sh singularity/small-bench.sh
```

If an exhaustive execution is required for REAL benchmarking :tm:, perform the following
```sh
sh singularity/big-bench.sh
```

[1]: https://cmake.org/download/
[2]: https://cmake.org/cmake/help/latest/manual/cmake.1.html#install-a-project
[3]: https://cmake.org/cmake/help/latest/command/find_package.html
