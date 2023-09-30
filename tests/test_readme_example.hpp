#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

// Makes sure that the example in the README works as expected.
// It would be embarrassing if it didn't...
void test_readme_example() {
    // "firstName", "lastName" and "children" are the field names
    // as they will appear in the JSON. The C++ standard is
    // snake case, the JSON standard is camel case, so the names
    // will not always be identical.
    struct Person {
        rfl::Field<"firstName", std::string> first_name;
        rfl::Field<"lastName", std::string> last_name;
        rfl::Field<"children", std::vector<Person>> children;
    };

    const auto bart = Person{.first_name = "Bart",
                             .last_name = "Simpson",
                             .children = std::vector<Person>()};

    const auto lisa = Person{
        .first_name = "Lisa",
        .last_name = "Simpson",
        .children = rfl::default_value  // same as std::vector<Person>()
    };

    // Returns a deep copy of the original object,
    // replacing first_name.
    const auto maggie =
        rfl::replace(lisa, rfl::make_field<"firstName">(std::string("Maggie")));

    const auto homer =
        Person{.first_name = "Homer",
               .last_name = "Simpson",
               .children = std::vector<Person>({bart, lisa, maggie})};

    // We can now transform this into a JSON string.
    const std::string json_string = rfl::json::write(homer);

    std::cout << json_string << std::endl;

    // And we can parse the string to recreate the struct.
    auto homer2 = rfl::json::read<Person>(json_string).value();

    // Fields can be accessed like this:
    std::cout << "Hello, my name is " << homer.first_name() << " "
              << homer.last_name() << "." << std::endl;

    // Since homer2 is mutable, we can also change the values like this:
    homer2.first_name = "Marge";

    std::cout << "Hello, my name is " << homer2.first_name() << " "
              << homer2.last_name() << "." << std::endl;
}
