#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_anonymous_fields() {
  std::cout << "test_anonymous_fields" << std::endl;

  using Age = rfl::Validator<unsigned int,
                             rfl::AllOf<rfl::Minimum<0>, rfl::Maximum<130>>>;

  struct Person {
    std::string first_name;
    std::string last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;
    unsigned int age;
    std::string email;
    std::vector<Person> children;
  };

  const auto bart = Person{.first_name = "Bart",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .age = 10,
                           .email = "bart@simpson.com",
                           .children = std::vector<Person>()};

  const auto lisa = Person{.first_name = "Lisa",
                           .last_name = "Simpson",
                           .birthday = "1987-04-19",
                           .age = 8,
                           .email = "lisa@simpson.com"};

  const auto maggie = Person{.first_name = "Maggie",
                             .last_name = "Simpson",
                             .birthday = "1987-04-19",
                             .age = 0,
                             .email = "maggie@simpson.com"};

  const auto homer =
      Person{.first_name = "Homer",
             .last_name = "Simpson",
             .birthday = "1987-04-19",
             .age = 45,
             .email = "homer@simpson.com",
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(
      homer,
      R"(["Homer","Simpson","1987-04-19",45,"homer@simpson.com",[["Bart","Simpson","1987-04-19",10,"bart@simpson.com",[]],["Lisa","Simpson","1987-04-19",8,"lisa@simpson.com",[]],["Maggie","Simpson","1987-04-19",0,"maggie@simpson.com",[]]]])");
}
