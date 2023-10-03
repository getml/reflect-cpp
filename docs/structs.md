# Structs

In order to setup a struct, all of it fields need to be an `rfl::Field`. The `rfl::Field` contains the type and the name. 
The name is a compile-time string and won't have any runtime overhead. The compile-time string is what will eventually
appear in the JSON (or any other format).

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"children", std::vector<Person>> children;
};
```

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
