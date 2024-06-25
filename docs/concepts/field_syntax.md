## The `rfl::Field`-syntax 

As we have shown, reflect-cpp can automatically retrieve field names from structs.

However, it also possible to annotate the fields using `rfl::Field`, like this:

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"children", std::vector<Person>> children;
};
```

These annotations are required if you are planning to use a compiler other than MSVC, GCC or clang.

There are a two things you have to keep in mind:

!!! tip 1. If you annotate *some* of the fields using `rfl::Field`, then you must annotate *all* of them.

2. If you are combining structs using `rfl::Flatten`, then they structs combined like this must either be annotated or not.

You can initilize your struct like this:

```cpp
const auto bart = Person{.first_name = "Bart",
                         .last_name = "Simpson",
                         .children = std::vector<Person>()};
```

Note that all fields must be explicitly set. This is on purpose, because it prevents you from accidentally forgetting fields,
which can lead to tricky runtime errors.

You can initialize a field using `rfl::default_value`:

```cpp
const auto bart = Person{.first_name = "Bart",
                         .last_name = "Simpson",
                         .children = rfl::default_value};
```

You can also initialize fields in the struct declaration itself:

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name = "Simpson";
    rfl::Field<"children", std::vector<Person>> children = rfl::default_value;
};
```

Because you now have explicitly set those fields, you do not have to do this every time:

```cpp
const auto bart = Person{.first_name = "Bart"};

const auto lisa = Person{.first_name = "Lisa"};

const auto maggie = Person{.first_name = "Maggie"};

const auto homer = Person{.first_name = "Homer",
                          .children = std::vector<Person>({bart, lisa, maggie})};

```

You can write this to a JSON string:

```cpp
const auto json_string = rfl::json::write(homer);
```

```json
{"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson","children":[]},{"firstName":"Lisa","lastName":"Simpson","children":[]},{"firstName":"Maggie","lastName":"Simpson","children":[]}]}
```

And you can parse it back into a struct:

```cpp
const auto homer = rfl::json::read<Person>(json_string).value();
```

## Flatten structs

In some cases, you want to structs to "inherit" fields from other structs. Unfortunately, doing this
via actual inheritance is not possible at the moment, due to several restrictions imposed by C++. 
Instead, we do it via composition, using `rfl::Flatten`.

Note that you can also have nested flattened fields. If `A` contains `B` and `B` contains `C`, then
all fields will be flattened to a single JSON object.

If there are duplicate field names, you will get a compile-time error.

** Example: Every employee is a person **

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

** Example, using the `rfl::Field`-syntax.**

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


## Anonymous fields

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

### Struct flattening with anonymous fields

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

## Optional fields

Usually, when you set up a struct, all of the fields are required. For instance, in the previous example,
we generated a JSON string like this:

```json
{"firstName":"Bart","lastName":"Simpson","children":[]}
```

Likewise, if we read the JSON string and the field "children" is missing, that will result in a runtime error.

But Bart doesn't have any children. After all, he's only 10 years old. To indicate to the library that the field "children" is optional,
we can use either `std::optional`, `std::shared_ptr` or `std::unique_ptr`.

If `std::optional` is `std::nullopt` or any of the smart pointers is a `nullptr`, 
then the field will not be written to the JSON string. Likewise, when you read the JSON string, the fields will not be required. If you
want the fields to be required, you can use the `rfl::NoOptionals` processor, please refer to the section on processors.

So this is how we could rewrite the previous example:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name = "Simpson";

    // Indicates to the library that the field is optional.
    std::optional<std::vector<Person>> children;
};

const auto bart = Person{.first_name = "Bart"};

const auto lisa = Person{.first_name = "Lisa"};

const auto maggie = Person{.first_name = "Maggie"};

const auto homer = Person{.first_name = "Homer",
                          .children = std::vector<Person>({bart, lisa, maggie})};

const auto json_string = rfl::json::write(homer);
```

Now, the field "children" is missing for Bart, Lisa and Maggie:

```json
{"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson"},{"firstName":"Lisa","lastName":"Simpson"},{"firstName":"Maggie","lastName":"Simpson"}]}
```
