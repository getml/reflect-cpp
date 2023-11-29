# Optional fields

Usually, when you set up a struct, all of the fields are required. For instance, in the previous example,
we generated a JSON string like this:

```json
{"firstName":"Bart","lastName":"Simpson","children":[]}
```

Likewise, if we read the JSON string and the field "children" is missing, that will result in a runtime error.

But Bart doesn't have any children. After all, he's only 10 years old. To indicate to the library that the field "children" is optional,
we can use either `std::optional`, `std::shared_ptr` or `std::unique_ptr`.

If `std::optional` is `std::nullopt` or any of the smart pointers is a `nullptr`, 
then the field will not be written to the JSON string. Likewise, when you read the JSON string, the fields will not be required.

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
