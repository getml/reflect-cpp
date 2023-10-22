# Supporting your own format

In order to support your own serialization format, you need to implement a reader and a writer for your concept.

The reader and writer constitute a thin layer between the serialization library of your choice and reflect-cpp.

The requirements are laid down in the concepts [IsReader](https://github.com/getml/reflect-cpp/blob/main/include/rfl/parsing/IsReader.hpp) and
[IsWriter](https://github.com/getml/reflect-cpp/blob/main/include/rfl/parsing/IsWriter.hpp) and also documented below.

Using the reader and the writer you can define your parser like this:

```cpp
template <class T>
using YourParser = rfl::parsing::Parser<YourReader, YourWriter, T>;
```

This can then be used to implement a `read` function and a `write` function:

```cpp
using InputVarType = typename YourReader::InputVarType;
using OutputVarType = typename YourWriter::OutputVarType;

template <class T>
rfl::Result<T> read(const std::string& _str) {
    // This should be supported by whatever library you are
    // using for your format.
    const InputVarType root = str_to_input_var(_str);

    // You can pass variables to the constructor, if necessary
    const auto r = Reader(...); 

    return YourParser<T>::read(r, root);
}

template <class T>
std::string write(const T& _obj) {
    // You can pass variables to the constructor, if necessary
    auto w = Writer(...);

    OutputVarType var = Parser<T>::write(w, _obj);

    // This should be supported by whatever library you are
    // using for your format.
    return output_var_to_str(var);
}
```

In the following two sections, we will provide templates for your `Reader` and `Writer`.
You should probably just Copy + Paste this into your own code and fill in the blanks.

## Implementing your own writer 

Because writers are somewhat simpler, we will start with them.

Any Writer needs to define the following types:

1) An `OutputArrayType`, which must be an array-like data structure.
2) An `OutputObjectType`, which must contain key-value pairs.
3) An `OutputVarType`, which must be able to represent either
   OutputArrayType, OutputObjectType or a basic type (bool, integral,
   floating point, std::string). We hesitate to call these "primitive types",
   because primitive types in C++ are defined as a slightly different group
   of types.

It also needs to support the following methods:

```cpp
struct Writer {
    using OutputArrayType = ...;
    using OutputObjectType = ...;
    using OutputVarType = ...;

    /// Appends `_var` to the end of `_arr`, thus mutating it.
    void add(const OutputVarType _var, OutputArrayType* _arr) const noexcept {...}

    /// Returns an empty OutputVarType, the NULL type of the format.
    OutputVarType empty_var() const noexcept {...}

    /// Generates an OutputVarType from a basic type
    /// (std::string, bool, floating point or integral).
    template <class T>
    OutputVarType from_basic_type(const T& _var) const noexcept {...}

    /// Generates a new, empty array.
    OutputArrayType new_array() const noexcept {...}

    /// Generates a new, empty object.
    OutputObjectType new_object() const noexcept {...}

    /// Determines whether the var is empty (whether it is the NULL type).
    bool is_empty(const OutputVarType& _var) const noexcept {...}

    /// Adds a new field to obj, thus mutating it.
    void set_field(const std::string& _name, const OutputVarType& _var,
                   OutputObjectType* _obj) const noexcept {...}
};
```

## Implementing your own reader 
