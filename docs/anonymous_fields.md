# Anonymous fields

If you do not want to serialize field names, you do not have to use `rfl::Field`. You can just write your struct like this:

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  std::vector<Person> children;
};
```

This is referred to as anonymous fields. It is particularly relevant if you are using a binary format.

This can be fully serialized and deserialized using `rfl::json::read` and `rfl::json::write`. No further annotations are needed.

However, there are a two things you have to keep in mind:

1) If you annotate *some* of the fields using either `rfl::Field`, then you must annotate *all* of them using one of these two.
2) If you are combining structs using `rfl::Flatten`, then they structs combined like this must either be annotated or not.

## Struct flattening with anonymous fields

Struct flattening also works with anonymous fields:

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  int age;
};

struct Employee {
  rfl::Flatten<Person> person;
  std::string employer;
  float salary;
};

const auto employee = Employee{
    .person = Person{.first_name = "Homer",
                     .last_name = "Simpson",
                     .age = 45},
    .employer = "Mr. Burns",
    .salary = 60000.0};
```

This will result in the following JSON:

```json
["Homer","Simpson",45,"Mr. Burns",60000.0]
```

You can even include `std::tuple` (but `std::tuple` itself cannot contain `rfl::Flatten`):

```cpp
using Person = std::tuple<std::string, std::string, int>;

struct Employee {
  rfl::Flatten<Person> person;
  std::string employer;
  float salary;
};

const auto employee = Employee{
    .person = Person{"Homer", "Simpson", 45},
    .employer = "Mr. Burns",
    .salary = 60000.0};
```

This will result in the same JSON:

```json
["Homer","Simpson",45,"Mr. Burns",60000.0]
```

