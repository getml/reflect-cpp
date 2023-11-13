#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_multiset() {
    std::cout << "test_multiset" << std::endl;

    struct Person {
        rfl::Field<"firstName", std::string> first_name;
        rfl::Field<"lastName", std::string> last_name = "Simpson";
        rfl::Field<"children", std::unique_ptr<std::multiset<std::string>>>
            children = rfl::default_value;
    };

    auto children = std::make_unique<std::multiset<std::string>>(
        std::multiset<std::string>({"Bart", "Lisa", "Maggie"}));

    const auto homer =
        Person{.first_name = "Homer", .children = std::move(children)};

    write_and_read(homer);
}
