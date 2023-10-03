#include <array>
#include <iostream>
#include <memory>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

void test_array() {
    std::cout << "test_array" << std::endl;

    struct Person {
        rfl::Field<"firstName", std::string> first_name;
        rfl::Field<"lastName", std::string> last_name = "Simpson";
        rfl::Field<"children", std::shared_ptr<std::array<Person, 3>>>
            children = rfl::default_value;
    };

    const auto bart = Person{.first_name = "Bart"};

    const auto lisa = Person{.first_name = "Lisa"};

    const auto maggie = Person{.first_name = "Maggie"};

    const auto homer =
        Person{.first_name = "Homer",
               .children = std::make_shared<std::array<Person, 3>>(
                   std::array<Person, 3>{bart, lisa, maggie})};

    write_and_read(
        homer,
        "{\"firstName\":\"Homer\",\"lastName\":\"Simpson\",\"children\":[{"
        "\"firstName\":\"Bart\",\"lastName\":\"Simpson\"},{"
        "\"firstName\":\"Lisa\",\"lastName\":\"Simpson\"},{"
        "\"firstName\":\"Maggie\",\"lastName\":\"Simpson\"}]}");
}
