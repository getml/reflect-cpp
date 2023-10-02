#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// Makes sure that we can set default values.
void test_literal() {
    std::cout << "test_literal" << std::endl;

    using FirstName = rfl::Literal<"Homer", "Marge", "Bart", "Lisa", "Maggie">;
    using LastName = rfl::Literal<"Simpson">;

    struct Person {
        rfl::Field<"firstName", FirstName> first_name;
        rfl::Field<"lastName", LastName> last_name = rfl::default_value;
        rfl::Field<"children", std::vector<Person>> children =
            rfl::default_value;
    };

    const auto bart = Person{.first_name = FirstName::make<"Bart">()};

    write_and_read(
        bart,
        "{\"firstName\":\"Bart\",\"lastName\":\"Simpson\",\"children\":[]}");
}
