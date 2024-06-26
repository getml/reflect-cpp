# `rfl::Generic` 

reflect-cpp is intended to be used to directly parse into structs. Doing this is more efficient, more convenient and also safer.

But in some cases, we simply cannot anticipate the complete structure of the data at compile time. For these cases, we
have `rfl::Generic`.

`rfl::Generic` is a convenience wrapper around the following type:

```cpp
std::variant<bool, int, double, std::string, Object, Array>;
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

You can retrieve the underlying `std::variant` using `.get()`. This then allows you to handle allow possible six cases using the [visitor pattern](https://en.cppreference.com/w/cpp/utility/variant/visit).

If you have a guess what the types of a particular field might be, you can use any of the six convenience methods `.to_array()`, `.to_bool()`, `.to_double()`, `.to_int()`, `.to_object()`, `.to_string()`. Each
of these methods will return an `rfl::Result<...>` with the corresponding type, if the `rfl::Generic` does indeed contain such a type.

If you prefer, you can also use the following six convenience *functions*:

`rfl::to_array(...)`, `rfl::to_bool(...)`, `rfl::to_double(...)`, `rfl::to_int(...)`, `rfl::to_object(...)`, `rfl::to_string(...)`.

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
