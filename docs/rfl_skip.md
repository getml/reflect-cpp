# `rfl::Skip`  

It is possible to skip the serialization and deserialization of fields using `rfl::Skip`.

```cpp
struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::Skip<std::string> town;
  Age age;
};
```

This means that the field `town` will be ignored:

```json
{"firstName":"Homer","lastName":"Simpson","age":45}
```

If you only want to skip the deserialization, but not the serialization, you can use `rfl::SkipDeserialization`:


```cpp
struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::SkipDeserialization<std::string> town;
  Age age;
};
```

If you only want to skip the serialization, but not the deserialization, you can use `rfl::SkipSerialization`. 
Note that this implies that the serializied data cannot be deserialized, because the field `town`
is expected during deserialization, but not serialized.

```cpp
struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name;
  rfl::SkipSerialization<std::string> town;
  Age age;
};
```

You can access the underlying value in the field `town` using any of the following operators:

```cpp
person.town();
person.town.get();
person.town.value();
```

You can assign the underlying field just like any other field:

```cpp
person.town = "Springfield";
```
