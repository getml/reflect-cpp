# `rfl::Generic` 

reflect-cpp is intended to be used to directly parse into structs. Doing this is more efficient, more convenient and also safer.

But in some cases, we simply cannot anticipate the complete structure of the data at compile time. For these cases, we
have `rfl::Generic`.

Note that generics are not supported for Avro.

`rfl::Generic` is a convenience wrapper around the following type:

```cpp
std::variant<bool, int, double, std::string, Object, Array, std::nullopt_t>;
```

`Object` and `Array` are defined as follows:

```cpp
using Array = std::vector<Generic>;
using Object = rfl::Object<Generic>;
```

In other words, `rfl::Generic` encompasses all types that can be expressed in JSON.

It can be used as follows:

```cpp
auto bart = rfl::Generic::Object();
bart["first_name"] = "Bart";
bart["last_name"] = "Simpson";
bart["age"] = 10;

auto lisa = rfl::Generic::Object();
lisa["first_name"] = "Lisa";
lisa["last_name"] = "Simpson";
lisa["age"] = 8;

auto maggie = rfl::Generic::Object();
maggie["first_name"] = "Lisa";
maggie["last_name"] = "Simpson";
maggie["age"] = 0;

auto homer = rfl::Generic::Object();
homer["first_name"] = "Homer";
homer["last_name"] = "Simpson";
homer["age"] = 45;
homer["children"] = rfl::Generic::Array({bart, lisa, maggie});
```

The resulting JSON strings looks as follows:

```json
{"first_name":"Homer","last_name":"Simpson","age":45,"children":[{"first_name":"Bart","last_name":"Simpson","age":10},{"first_name":"Lisa","last_name":"Simpson","age":8},{"first_name":"Lisa","last_name":"Simpson","age":0}]}
```

`rfl::Generic` contains some convenience methods that allow you to handle parsed data:

You can retrieve the underlying `std::variant` using `.get()`. This then allows you to handle allow possible seven cases using the [visitor pattern](https://en.cppreference.com/w/cpp/utility/variant/visit).

If you have a guess what the types of a particular field might be, you can use any of the seven convenience methods `.to_array()`, `.to_bool()`, `.to_double()`, `.to_int()`, `.to_null()`, `.to_object()`, `.to_string()`. Each
of these methods will return an `rfl::Result<...>` with the corresponding type, if the `rfl::Generic` does indeed contain such a type.

If you prefer, you can also use the following seven convenience *functions*:

`rfl::to_array(...)`, `rfl::to_bool(...)`, `rfl::to_double(...)`, `rfl::to_int(...)`, `rfl::to_null`, `rfl::to_object(...)`, `rfl::to_string(...)`.

These functions are particularly useful for monadic error handling:

```cpp
auto homer = rfl::Generic::Object();
...
rfl::Result<int> age = homer.get("age").and_then(rfl::to_int);
```

In this particular instance, we know that `homer` is an `rfl::Generic::Object`. We assume that it has a field named `age` which we also assume is an integer.
In the end, we will get an `rfl::Result<int>` which will contain an integer, if both of our assumptions are correct and an error otherwise.

If we want to retrieve the age, we can use the `.value()` method:


```cpp
auto homer = rfl::Generic::Object();
...
int age = homer.get("age").and_then(rfl::to_int).value();
```

This will throw an exception if any of our assumptions are false. 

Please refer to the part of the documentation on `rfl::Result` to learn more about monadic error handling.

## `rfl::to_generic` and `rfl::from_generic`

You can transform any struct that can be converted to a JSON to the equivalent generic instead.

In other words:

```cpp
assert(rfl::json::write(my_struct) == rfl::json::write(rfl::to_generic(my_struct)));
```

And:

```cpp
assert(rfl::json::read<rfl::Generic>(rfl::json::write(my_struct)).value() == rfl::to_generic(my_struct));
```

What is more, you can apply any processors you want to:

```cpp
assert(rfl::json::write<rfl::SnakeCaseToCamelCase>(my_struct) == rfl::json::write(rfl::to_generic<rfl::SnakeCaseToCamelCase>(my_struct)));

assert(rfl::json::read<rfl::Generic>(rfl::json::write<rfl::SnakeCaseToCamelCase>(my_struct)).value() == rfl::to_generic<rfl::SnakeCaseToCamelCase>(my_struct));
```

Conversely, the same statements hold for reading. In other words:

```cpp
assert(rfl::json::read<MyStruct>(my_json_string).value() == rfl::from_generic<MyStruct>(rfl::json::read<rfl::Generic>(my_json_string).value()).value());
```

Likewise, you can apply processors:

```cpp
assert(rfl::json::read<MyStruct, rfl::SnakeCaseToCamelCase>(my_json_string).value() == rfl::from_generic<MyStruct, rfl::SnakeCaseToCamelCase>(rfl::json::read<rfl::Generic>(my_json_string).value()).value());
```

In fact, you can think of `rfl::from_generic` and `rfl::to_generic` as manifestations of the reader and writer interface. You could as well write `rfl::generic::read<...>(...)` and `rfl::generic::write(...)`.

This can be used to dynamically build structs:

```cpp
struct Person {
    std::string first_name;
    std::string last_name;
    int age;
};

auto bart = rfl::Generic::Object();
bart["first_name"] = "Bart";
bart["last_name"] = "Simpson";
bart["age"] = 10;

const auto person = rfl::from_generic<Person>(bart).value();
```

