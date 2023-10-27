# Support for standard containers

The serialization and deserialization of containers is handled as follows:

`std::map<std::string, ...>` and `std::unordered_map<std::string, ...>` are treated
as objects.

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

All other supported standard containers 
(other than `std::variant`, `std::optional`, `std::unique_ptr` and `std::shared_ptr`) 
will be represented as arrays. Containers for which the `value_type`
is a key-value-pair, will be represented as arrays of pairs. 

Consider the following map:

```cpp
std::map<int, Person> children;
children[1] = Person{.first_name = "Bart", .last_name = "Simpson"};
children[2] = Person{.first_name = "Lisa", .last_name = "Simpson"};
children[3] = Person{.first_name = "Maggie", .last_name = "Simpson"};
```

This will be represented as follows:

```json
[[1,{"firstName":"Bart","lastName":"Simpson"}],[2,{"firstName":"Lisa","lastName":"Simpson"}],[3,{"firstName":"Maggie","lastName":"Simpson"}]]
```

