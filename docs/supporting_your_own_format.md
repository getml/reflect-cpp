# Supporting your own format

In order to support your own serialization format, you need to implement a reader and a writer on top of whatever serialization libary
you have chosen.

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

Any Reader needs to define the following:

1) An `InputArrayType`, which must be an array-like data structure.
2) An `InputObjectType`, which must contain key-value pairs.
3) An `InputVarType`, which must be able to represent either
   `InputArrayType`, `InputObjectType` or a basic type (bool, integral,
   floating point, std::string).
4) A `static constexpr bool has_custom_constructor<T>`, that determines
   whether the class in question as a custom constructor, which might
   be called something like `from_json_obj(...)`. If you do not want to
   support this functionality, just set it to false.
   
It also needs to support the following methods:

```cpp
struct Reader {
    using InputArrayType = ...;
    using InputObjectType = ...;
    using InputVarType = ...;

    /// If you do not want to support custom constructors,
    /// just set this to false.
    template <class T>
    static constexpr bool has_custom_constructor = false;

    /// Retrieves a particular field from an object.
    /// Returns an rfl::Error if the field cannot be found.
    rfl::Result<InputVarType> get_field(
        const std::string& _name, const InputObjectType& _obj) const noexcept {...}

    /// Determines whether a variable is empty (the NULL type).
    bool is_empty(const InputVarType& _var) const noexcept {...}

    /// Cast _var as a basic type (bool, integral,
    /// floating point, std::string).
    /// Returns an rfl::Error if it cannot be cast
    /// as that type
    template <class T>
    rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {...}

    /// Casts _var as an InputArrayType.
    /// Returns an rfl::Error if `_var` cannot be cast as an array.
    rfl::Result<InputArrayType> to_array(const InputVarType& _var) const noexcept {...}

    /// fct is a function that turns the field name into the field index of the
    /// struct. It returns -1, if the fields does not exist on the struct. This
    /// returns an std::array that can be used to build up the struct.
    /// See below for a more comprehensive explanation.
    template <size_t size, class FunctionType>
    std::array<InputVarType, size> to_fields_array(
        const FunctionType _fct, const InputObjectType& _obj) const noexcept {...}

    /// Iterates through an object and writes the contained key-value pairs into
    /// a vector.
    std::vector<std::pair<std::string, InputVarType>> to_map(
        const InputObjectType& _obj) const noexcept {...}

    /// Casts _var as an InputObjectType.
    /// Returns an rfl::Error if `_var` cannot be cast as an object.
    rfl::Result<InputObjectType> to_object(
        const InputVarType& _var) const noexcept {...}

    /// Iterates through an array and writes the contained values into
    /// a std::vector<InputVarType>. 
    std::vector<InputVarType> to_vec(const InputArrayType& _arr) const noexcept {...}

    /// Constructs T using its custom constructor. This will only be triggered if
    /// T was determined to have a custom constructor by
    /// static constexpr bool has_custom_constructor, as defined above.
    /// Returns an rfl::Error, if the custom constructor throws an exception.
    template <class T>
    rfl::Result<T> use_custom_constructor(
        const InputVarType& _var) const noexcept {
        // If you do not want to support this functionality,
        // just return this.
        return rfl::Error("Not supported.");
    }
};
```

Of these methods, `to_fields_array` probably requires futher explanation.

Consider the following struct:

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
    rfl::Field<"children", std::vector<Person>> children;
};
```

This struct contains four fields. `rfl::parsing::Parser` expects `to_fields_array`
to return an `std::array<InputVarType, 4>` containing the field "firstName" in the
first position, "lastName" in the second position, "birthday" in the third position
and "children" in the fourth position.

`rfl::parsing::Parser` will pass the following two elements:

1. A function `_fct` of type `FunctionType`.
2. An object `_obj` of type `InputObjectType`.

`_fct` takes a field name of type `std::string_view` as an input and 
returns an index. This index signifies the position in the `std::array` 
it wants this field to have. If the field is not contained in the struct,
the index will be -1.

Your job is to implement the following:

1. Iterate through `_obj`.
2. Identify the required index of the field name using `_fct`.
3. Set the corresponding field in `std::array` to the field value associated with the field name.
4. Any field that could not be set in steps 1-3 must be set to the NULL value,
   such that `Reader.is_empty(...)` would return `true`.

