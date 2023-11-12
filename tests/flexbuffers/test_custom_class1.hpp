#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_custom_class1() {
    std::cout << "test_custom_class1" << std::endl;

    struct Person {
        struct PersonImpl {
            rfl::Field<"firstName", std::string> first_name;
            rfl::Field<"lastName", std::string> last_name = "Simpson";
            rfl::Field<"children", std::vector<Person>> children =
                rfl::default_value;
        };

        using ReflectionType = PersonImpl;

        Person(const PersonImpl& _impl) : impl(_impl) {}

        Person(const std::string& _first_name)
            : impl(PersonImpl{.first_name = _first_name}) {}

        const ReflectionType& reflection() const { return impl; };

       private:
        PersonImpl impl;
    };

    const auto bart = Person("Bart");

    write_and_read(bart);
}
