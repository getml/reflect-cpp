# Flatten structs

In some cases, you want to structs to "inherit" fields from other structs. Unfortunately, doing this
via actual inheritance is not possible at the moment, due to several restrictions imposed by C++. 
Instead, we do it via composition, using `rfl::Flatten`.

Note that you can also have nested flattened fields. If `A` contains `B` and `B` contains `C`, then
all fields will be flattened to a single JSON object.

If there are duplicate field names, you will get a compile-time error.

## Example: Every employee is a person

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

const auto json_str = rfl::json::write(employee);
```

This flattens all fields into a single JSON object:

```json
{"first_name":"Homer","last_name":"Simpson","age":45,"salary":60000.0}
```

## Example, using the `rfl::Field`-syntax.

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"age", int> age;
};

struct Employee {
    rfl::Flatten<Person> person;
    rfl::Field<"salary", float> salary;
};

const auto employee = Employee{
    .person = Person{.first_name = "Homer", .last_name = "Simpson", .age = 45},
    .salary = 60000.0
};

const auto json_str = rfl::json::write(employee);
```

This flattens all fields into a single JSON object:

```json
{"firstName":"Homer","lastName":"Simpson","age":45,"salary":60000.0}
```

