# Benchmarks

reflect-cpp conducts continuuous benchmarking across different operating systems, compilers and architectures
and publishes the results on its [Actions tab](https://github.com/getml/reflect-cpp/actions).
Refer to the [benchmarks folder](https://github.com/getml/reflect-cpp/tree/main/benchmarks) for details.

The benchmarks show that reflect-cpp is not only very convenient, but also one the fastest JSON libraries for C++.
It is faster than RapidJSON and about 10 times faster than nlohmann/json. It can be even faster than that, 
if you choose to use a different format supported by reflect-cpp, such as msgpack.