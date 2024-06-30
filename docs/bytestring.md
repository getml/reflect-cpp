# `rfl::Bytestring` 

`rfl::Bytestring` is a simple alias:

```cpp
namespace rfl {

using Bytestring = std::basic_string<std::byte>;

}
```

For more information on how to handle `std::basic_string` please refer to the
relevant documentation:

https://en.cppreference.com/w/cpp/string/basic_string

Bytestrings are supported by BSON, CBOR, flexbuffers and msgpack. Textual formats
do not support them.
