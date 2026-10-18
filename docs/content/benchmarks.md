# Benchmarks

reflect-cpp conducts continuuous benchmarking across different operating systems, compilers and architectures
and publishes the results on its [Actions tab](https://github.com/getml/reflect-cpp/actions).
Refer to the [benchmarks folder](https://github.com/getml/reflect-cpp/tree/main/benchmarks) for details.

The benchmarks show that reflect-cpp is not only very convenient, but also one the fastest JSON libraries for C++.
It is faster than RapidJSON and about 10 times faster than nlohmann/json. It can be even faster than that,
if you choose to use a different format supported by reflect-cpp, such as msgpack.

## Profiling with gperftools

reflect-cpp's benchmark suite supports [gperftools](https://github.com/gperftools/gperftools) for CPU profiling and call-tree analysis.

### 1. Install gperftools via vcpkg

```bash
vcpkg install gperftools
```

### 2. Install the Go pprof visualizer

The original Perl-based pprof was removed in gperftools 2.17. Install the Go version:

```bash
go install github.com/google/pprof@latest
```

Add `~/go/bin` to your `PATH` if it is not already.

### 3. Compile with gperftools support

To enable gperftools, pass `-DREFLECTCPP_USE_GPERFTOOLS=ON` to cmake:

```bash
cmake -S . -B build \
    -DCMAKE_CXX_STANDARD=20 \
    -DCMAKE_BUILD_TYPE=Release \
    -DREFLECTCPP_BUILD_BENCHMARKS=ON \
    -DREFLECTCPP_USE_GPERFTOOLS=ON \
    -DCMAKE_TOOLCHAIN_FILE=<vcpkg_root>/scripts/buildsystems/vcpkg.cmake
cmake --build build -j 4
```

### 4. Run a benchmark with CPU profiling

Set the `CPUPROFILE` environment variable. The profile is written to the current directory:

```bash
CPUPROFILE=benchmark.prof ./build/benchmarks/json/reflect-cpp-json-benchmarks --benchmark_filter=canada
```

### 5. View the call tree

```bash
pprof -call_tree ./build/benchmarks/json/reflect-cpp-json-benchmarks benchmark.prof
```

This produces a tree of functions ranked by sampled CPU time, showing exactly where time is spent inside reflect-cpp's JSON parser and serializers.
