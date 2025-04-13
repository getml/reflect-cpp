# `rfl::Timestamp` and `std::chrono::duration`

## `rfl::Timestamp`

`rfl::Timestamp` is a simple wrapper around `std::tm` (https://en.cppreference.com/w/cpp/chrono/c/tm).

As its template parameter it includes the time stamp format for serialization and deserialization.

Here is a reference for for how you can specify the format:

https://en.cppreference.com/w/cpp/chrono/c/strftime

In fact, `rfl::Timestamp` calls `strftime` and `strptime` under-the-hood.

They can be used in your struct like this:

```cpp
struct Person {
  rfl::Timestamp<"%Y-%m-%d"> birthday;
};
```

An `rfl::Timestamp` can be constructed from either a string or a `std::tm` struct.

```cpp
const auto person1 = Person{.birthday = "1970-01-01"};
const auto person2 = Person{.birthday = std::tm{...}};
```

You can access the underlying `std::tm` struct using the `.tm()` method and you can generate 
the string representation using the `.str()` method.

```cpp
const std::tm birthday = person1.birthday.tm();
std::cout << person1.birthday.str() << std::endl;
```

Note that constructing `rfl::Timestamp` from a string is convenient, but it might result in
an exception.

To avoid the exception you can also use `Timestamp<...>::from_string(...)`, which will
return an `rfl::Result<Timestamp<...>>` or `rfl::Error`.

```cpp
const rfl::Result<rfl::Timestamp<"%Y-%m-%d">> result = rfl::Timestamp<"%Y-%m-%d">::from_string("1970-01-01");
const rfl::Result<rfl::Timestamp<"%Y-%m-%d">> error = rfl::Timestamp<"%Y-%m-%d">::from_string("not a proper time format");
```

## `std::chrono::duration`

`std::chrono::duration` types are serialized as an object with the count and unit as fields:

```cpp
struct MyStruct {
  std::chrono::seconds duration;
};
rfl::json::write(MyStruct{.duration = std::chrono::seconds(10)});
```

This results in the following JSON:

```json
{"duration":{"count":10,"unit":"seconds"}}
```

Units are automatically transformed upon reading. For instance,
both of the JSON snippets below can be read into `MyStruct`, and both
yield equivalent results:

```json
{"duration":{"count":600,"unit":"seconds"}}
```

```json
{"duration":{"count":10,"unit":"minutes"}}
```

The second JSON snippet, encoded in minutes, will be transformed to
seconds.

The following units are supported: `std::chrono::nanoseconds`, `std::chrono::microseconds`,
`std::chrono::milliseconds`, `std::chrono::seconds`, 
`std::chrono::minutes`, `std::chrono::hours`, `std::chrono::days`,
`std::chrono::weeks`, `std::chrono::months`, and `std::chrono::years`.
