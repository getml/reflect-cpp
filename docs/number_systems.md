# `rfl::Binary`, `rfl::Hex` and `rfl::Oct` 

`rfl::Binary`, `rfl::Hex` and `rfl::Oct` can be used to express numbers
in binary, hex and octal format.

```cpp
struct ExampleStruct {
  rfl::Binary<uint8_t> binary;
  rfl::Hex<int> hex;
  rfl::Oct<int> oct;
};

const auto example = ExampleStruct{
    .binary = 30,
    .hex = 30,
    .oct = 30};
```

This results in the following JSON:

```json
{"binary":"00011110","hex":"1e","oct":"36"}
```

Note that the contained type must be integral for `rfl::Hex` and `rfl::Oct`. For `rfl::Binary`, it must be unsigned. Moreover, 
the number of digits for `rfl::Binary` will be determined by the bitsize of the type.

You can access the contained value using `.value()`, `.get()` or simply `operator()`.

You can produce the string representation using `.str()`.
