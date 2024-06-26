# `rfl::Object` 

`rfl::Object<...>` behaves similarly to `rfl::map<std::string, ...>` and `rfl::unordered_map<std::string, ...>`.

It will be represented in JSON or other formats as an object:

```cpp
auto bart = rfl::Object<std::string>();
bart["first_name"] = "Bart";
bart["last_name"] = "Simpson";
bart["town"] = "Springfield";
```

This is results in the following JSON string:

```cpp
{"first_name":"Bart","last_name":"Simpson","town":"Springfield"}
```

It supports almost all of the normal features you would expect from such a container. For instance, 
you can iterate through it just like any other map:

```cpp
for (const auto& [k, v]: bart) {
    std::cout << k << ": " << v << std::endl;
}
```

However, unlike these containers, the order of fields is preserved. It is also possible to have duplicate keys:


```cpp
auto bart = rfl::Object<std::string>();
bart["first_name"] = "Bart";
bart["last_name"] = "Simpson";
bart["town"] = "Springfield";

// Note that you need the .insert(...) method,
// because operator[] would overwrite the
// existing field.
bart.insert("first_name", "Lisa");
```

This results in the following JSON string:

```cpp
{"first_name":"Bart","last_name":"Simpson","town":"Springfield","first_name":"Lisa"}
```

There are three different ways of accessing fields:

- `operator[...]` creates a new field, if a field of this name doesn't already exist.
- `.at(...)` throws an exception, if field of this name doesn't exist.
- `.get(...)` returns an `rfl::Result` wrapping the field, or an `rfl::Error` if the field doesn't exist.
