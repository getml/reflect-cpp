# `rfl::Timestamp`

`rfl::Timestamp` is a simple wrapper around `std::tm` (https://en.cppreference.com/w/cpp/chrono/c/tm).

As its template parameter it includes the time stamp format for serialization and deserialization.

Here is a reference for for how you can specify the format:

https://en.cppreference.com/w/cpp/chrono/c/strftime

In fact, `rfl::Timestamp` does call `strftime` and `strptime` under-the-hood.

They can be used in your struct like this:

```cpp
struct Person {
  rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
};
```

An `rfl::Timestamp` can be constructed from either a string or a `std::tm` struct.

You can access the underlying `std::tm` struct using the `.tm()` method and you can generate 
the string representation using the `.str()` method.

Note that constructing `rfl::Timestamp` from a string is convenient, but it might result in
an exception.

To avoid the exception you can also use `Timestamp<...>::from_string(...)`, which will
return an `rfl::Result<Timestamp<...>>` or `rfl::Error`.
