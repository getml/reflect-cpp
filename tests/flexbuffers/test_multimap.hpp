#include <iostream>
#include <map>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>

#include "write_and_read.hpp"

void test_multimap() {
    std::cout << "test_multimap" << std::endl;

    struct Person {
        rfl::Field<"firstName", std::string> first_name;
        rfl::Field<"lastName", std::string> last_name = "Simpson";
        rfl::Field<"children", std::unique_ptr<std::multimap<int, Person>>>
            children = rfl::default_value;
    };

    auto children = std::make_unique<std::multimap<int, Person>>();
    children->insert(std::make_pair(1, Person{.first_name = "Bart"}));
    children->insert(std::make_pair(2, Person{.first_name = "Lisa"}));
    children->insert(std::make_pair(3, Person{.first_name = "Maggie"}));

    const auto homer =
        Person{.first_name = "Homer", .children = std::move(children)};

    write_and_read(homer);
}
