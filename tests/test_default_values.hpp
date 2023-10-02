#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// Makes sure that we can set default values.
void test_default_values() {
    std::cout << "Testing default values..." << std::endl;

    struct Person {
        rfl::Field<"firstName", std::string> first_name;
        rfl::Field<"lastName", std::string> last_name = "Simpson";
        rfl::Field<"children", std::vector<Person>> children =
            rfl::default_value;
    };

    const auto bart = Person{.first_name = "Bart"};

    write_and_read(
        bart,
        "{\"firstName\":\"Bart\",\"lastName\":\"Simpson\",\"children\":[]}");
}
