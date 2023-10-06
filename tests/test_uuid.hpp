#include <rfl.hpp>
#include "write_and_read.hpp"
void test_uuid() {
    std::cout << "test_uuid" << std::endl;


    struct Person {
        rfl::Field<"firstName", std::string> first_name;
        rfl::Field<"lastName", std::string> last_name = "Simpson";
        rfl::Field<"id", rfl::UUIDv1> id;
    };

    const auto bart = Person{.first_name = "Bart", .id = "12345678-1234-5678-1234-567812345678"};

    //write_and_read(bart,
    //               "{\"firstName\":\"Bart\",\"lastName\":\"Simpson\","
    //               "\"birthday\":\"1987-04-19\"}");
}
