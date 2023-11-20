# Support for standard containers

The serialization and deserialization of containers is handled as follows:

`std::map<std::string, ...>`, `std::unordered_map<std::string, ...>`, 
maps with any numeric types (integral and floating points) as keys,
and any objects where the `ReflectionType` is either a `std::string` or
a numeric type are treated as objects.

Consider the following map:

```cpp
std::map<std::string, std::string> homer;
homer["firstName"] = "Homer";
homer["lastName"] = "Simpson";
```

This will be represented as follows:

```json
{"firstName":"Homer","lastName":"Simpson"}
```

The same is true for numeric types:

```cpp
std::map<int, std::string> homer;
homer[1] = "Homer";
homer[2] = "Simpson";
```

This will be represented as follows:

```json
{"1":"Homer","2":"Simpson"}
```

If you wrap your keys into a `rfl::Validator`, this will be treated as an object as well:

```cpp
using Between1and3 = rfl::Validator<int, rfl::Minimum<1>, rfl::Maximum<3>>;
std::map<Between1and3, std::string> homer;
homer[1] = "Homer";
homer[2] = "Simpson";
```

This will also be represented as follows:

```json
{"1":"Homer","2":"Simpson"}
```

All other supported standard containers 
(other than `std::variant`, `std::optional`, `std::unique_ptr` and `std::shared_ptr`) 
will be represented as arrays. Containers for which the `value_type`
is a key-value-pair will be represented as arrays of pairs. 
