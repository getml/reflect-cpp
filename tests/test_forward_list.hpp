#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_forward_list() {
    std::cout << "test_forward_list" << std::endl;

    struct Person {
        rfl::Field<"firstName", std::string> first_name;
        rfl::Field<"lastName", std::string> last_name = "Simpson";
        rfl::Field<"children", std::unique_ptr<std::forward_list<Person>>>
            children = rfl::default_value;
    };

    auto children = std::make_unique<std::forward_list<Person>>();
    children->emplace_front(Person{.first_name = "Maggie"});
    children->emplace_front(Person{.first_name = "Lisa"});
    children->emplace_front(Person{.first_name = "Bart"});

    const auto homer =
        Person{.first_name = "Homer", .children = std::move(children)};

    write_and_read(
        homer,
        R"({"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson"},{"firstName":"Lisa","lastName":"Simpson"},{"firstName":"Maggie","lastName":"Simpson"}]})");
}
