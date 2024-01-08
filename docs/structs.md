# Structs

Structs can be set up as follows:

```cpp
struct Person {
    std::string first_name;
    std::string last_name;
    std::vector<Person> children;
};
```

You can initilize your struct like this:

```cpp
const auto bart = Person{.first_name = "Bart",
                         .last_name = "Simpson",
                         .children = std::vector<Person>()};
```

JSON uses Hungarian case, but C++ uses snake case, so you might want to rename your fields:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    std::vector<Person> children;
};
```


You can also initialize fields in the struct declaration itself:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name = "Simpson";
    std::vector<Person> children;
};
```

Because you now have explicitly set this field, you do not have to do this every time:

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

## Important note

**Do not create custom constructors on the structs.**

reflect-cpp needs to be able to create the structs like this:

```cpp
Person{"Bart", "Simpson", ...};
```

But if you create a custom constructor, then C++ will no longer allow this kind of constructions.

If you want to create the struct from one of your classes (the most like reason, you want to create custom constructors in the first place),
you might want to check out the section on [custom classes](https://github.com/getml/reflect-cpp/blob/main/docs/custom_classes.md) or [custom parsers](https://github.com/getml/reflect-cpp/blob/main/docs/custom_parser.md).

