# Benchmarks

reflect-cpp conducts continuuous benchmarking through its [Github Actions](https://github.com/getml/reflect-cpp/actions) pipelines for different compilers, operating systems 
and architectures. Currently, we benchmark the following:

1. How reflect-cpp's JSON parser compares to other JSON libraries for C or C++
2. How the different formats supported by reflect-cpp compare to each other

## Viewing the benchmark results

To view the benchmark results, go to the [Actions tab](https://github.com/getml/reflect-cpp/actions) 
of this Github repository, click on a recent run you are interested in and scroll down to view the summary.

## Running the benchmark results on your own machine

Make sure that you have vcpkg installed. Refer to the README in the root folder of
this repository to learn how to install vcpkg.

To compile the benchmarks, do the following:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DREFLECTCPP_BUILD_BENCHMARKS=ON
cmake --build build -j 4 # gcc, clang
cmake --build build --config Release -j 4 # MSVC
```

To run the benchmarks, do the following:

```
./build/benchmarks/json/reflect-cpp-json-benchmarks
./build/benchmarks/all/reflect-cpp-all-format-benchmarks
```

Note that the benchmarks are based on Google Benchmarks. You can use the `--benchmark_filter` flag
to only run some of the benchmarks:

```
./build/benchmarks/all/reflect-cpp-all-format-benchmarks --benchmark_filter=canada_read
```

## Results summary

### How reflect-cpp compares to other JSON libraries

- reflect-cpp is currently slightly slower than the fastest JSON libraries for C and C++, namely simdJSON and yyjson. We are working on getting that overhead down to zero.
- reflect-cpp is consistently faster than RapidJSON, usually about twice as fast.
- reflect-cpp is consistently about 10 times faster than nlohmann/json, which is undeniably the most popular JSON library for C++.

In summary, it is fair to say that reflect-cpp is among the fastest JSON libraries for C++. It also considerably more convenient: If you look at the benchmark code, you will find that you can implement the serialization and deserialization using a single line of code with reflect-cpp, but you will need about 50-90 lines of code to do the same with other libraries (including nlohmann/json).  

### How the different formats supported by reflect-cpp compare to each other

- flexbuffers and msgpack are the fastest two formats supported by reflect-cpp. They are particularly fast on datasets with a lot of numerical data, such as *canada*. flexbuffers is usually the fastest at reading data and msgpack is the fastest at writing.
- JSON is surprisingly fast. This is because reflect-cpp's JSON parser is built on top of yyjson, arguably the fastest JSON library for C or C++.
- The performance of CBOR is disappointing. We have tried swapping out the underlying library, TinyCBOR, for libcbor, but the results are similar.
- TOML and YAML are very slow when compared to the other formats. To be fair, these formats emphasize readability rather than speed.

In summary, if you are looking for a format that emphasizes speed, msgpack is probably your best bet. Not only does it perform very well on our benchmarks, but there are also many high-quality implementations in other languages. If you are looking for a good trade-off between speed and readability, you should probably just stick with JSON.
