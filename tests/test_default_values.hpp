#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// Makes sure that we can set default values.
void test_default_values() {
    std::cout << "test_default_values" << std::endl;

    struct Person {
        rfl::Field<"firstName", std::string> first_name;
        rfl::Field<"lastName", std::string> last_name = "Simpson";
        rfl::Field<"children", std::vector<Person>> children =
            rfl::default_value;
    };

    const auto bart = Person{.first_name = "Bart"};
    const auto lisa = Person{.first_name = "Lisa"};
    const auto maggie = Person{.first_name = "Maggie"};
    const auto homer =
        Person{.first_name = "Homer",
               .children = std::vector<Person>({bart, lisa, maggie})};

    write_and_read(
        homer,
        "{\"firstName\":\"Homer\",\"lastName\":\"Simpson\",\"children\":[{"
        "\"firstName\":\"Bart\",\"lastName\":\"Simpson\",\"children\":[]},{"
        "\"firstName\":\"Lisa\",\"lastName\":\"Simpson\",\"children\":[]},{"
        "\"firstName\":\"Maggie\",\"lastName\":\"Simpson\",\"children\":[]}]}");
}
