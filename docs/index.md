---
hide:
  # - toc
  - navigation
  - feedback
---


<div align="center" style="text-align: center;" markdown>

# ![C++](https://img.shields.io/badge/c++-%237156c2.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) reflect-cpp

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://GitHub.com/Naereen/StrapDown.js/graphs/commit-activity)
[![Generic badge](https://img.shields.io/badge/C++-20-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/gcc-11+-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/clang-14+-blue.svg)](https://shields.io/)
[![Generic badge](https://img.shields.io/badge/MSVC-17+-blue.svg)](https://shields.io/)
[![Conan Center](https://img.shields.io/conan/v/reflect-cpp)](https://conan.io/center/recipes/reflect-cpp)

</div>

 **💻 Source Code**: [https://github.com/getml/reflect-cpp](https://github.com/getml/reflect-cpp)

**reflect-cpp** is a C++-20 library for **fast serialization, deserialization and validation** using reflection, similar to [pydantic](https://github.com/pydantic/pydantic) in Python, [serde](https://github.com/serde-rs) in Rust, [encoding](https://github.com/golang/go/tree/master/src/encoding) in Go or [aeson](https://github.com/haskell/aeson/tree/master) in Haskell.

reflect-cpp fills an important gap in C++ development. It minimizes boilerplate code and enhances code safety for seamless and efficient data exchange across system components.

<h3>Design principles for reflect-cpp include:</h3>

- Close integration with [containers](https://github.com/getml/reflect-cpp?tab=readme-ov-file#support-for-containers) from the C++ standard library
- Close adherence to C++ idioms
- Out-of-the-box support for [JSON](https://rfl.getml.com/supported_formats/json)
- Simple [installation](https://rfl.getml.com/install)
- Simple extendability to [other serialization formats](https://rfl.getml.com/supported_formats/supporting_your_own_format)
- Simple extendability to [custom classes](https://rfl.getml.com/concepts/custom_classes)
- Being one of the fastest serialization libraries in existence, as demonstrated by our [benchmarks](https://rfl.getml.com/benchmarks)
  

## Why do we need this?

Suppose your C++ program has complex data structures it needs to save and load. Or maybe it needs to interact with some kind of external API. If you do this the traditional way, you will have a lot of boilerplate code. This is annoying and error-prone.

!!! example

    If you are new to reflect-cpp, please refer to our [GitHub repository](https://github.com/getml/reflect-cpp) for the latest list of supported [serialization formats]. You will also find a [simple] and [more comprehensive code example].

  [github repo]: https://github.com/getml/reflect-cpp
  [serialization formats]: https://github.com/getml/reflect-cpp#serialization-formats
  [simple]: https://github.com/getml/reflect-cpp#simple-example
  [more comprehensive code example]: https://github.com/getml/reflect-cpp#more-comprehensive-example

  
reflect-cpp is not just a reflection library, it is for **serialization, deserialization and validation** through reflection.

That means that you can encode your requirements about the input data in the type system and have them validated upfront. This is why the library also includes algebraic data types like tagged unions and numerous validation routines. Having your requirements encoded in the type system is the most reliable way of ensuring they are met. If your requirements are not met, the user of your software gets a very clear error message. Encoding your requirements in the type system also makes it a lot easier for anyone reading your code.

This increases user experience and developer experience, it makes your code safer (fewer bugs) and more secure (less prone to malicious attacks).

For a more in-depth theoretical discussions of these topics, the following books are warmly recommended:

- *Category Theory for Programmers* by Bartosz Milewski [on GitHub](https://github.com/hmemcpy/milewski-ctfp-pdf/releases)
- *Domain Modeling Made Functional* by Scott Wlaschin

## The team behind reflect-cpp

reflect-cpp has been developed by [getML (Code17 GmbH)](https://getml.com), a company specializing in software engineering and machine learning for enterprise applications. reflect-cpp is currently maintained by Patrick Urbanke and Manuel Bellersen, with major contributions coming from the community.

### Related projects

reflect-cpp was originally developed for [getml-community](https://github.com/getml/getml-community), the fastest open-source tool for feature engineering on relational data and time series. If you are interested in Data Science and/or Machine Learning, please check it out.

### Professional C++ support

For comprehensive C++ support beyond the scope of GitHub discussions, we’re here to help! Reach out at [support@getml.com](mailto:support%40getml.com?subject=C++%20support%20request) to discuss any technical challenges or project requirements. We’re excited to support your work as independent software consultants.


## License
reflect-cpp is released under the MIT License.
