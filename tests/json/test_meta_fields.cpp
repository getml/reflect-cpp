#include "test_meta_fields.hpp"

#include <iostream>
#include <rfl.hpp>

namespace test_meta_fields {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town = "Springfield";
  unsigned int age;
  std::vector<Person> children;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto fields = rfl::fields<Person>();

  /*std::cout << "Fields in " << rfl::type_name_t<Person>().str() << ":"
            << std::endl;
  for (const auto& f : fields) {
    std::cout << "name: " << f.name() << ", type: " << f.type() << std::endl;
  }*/

  // The exact content is somewhat unpredictable, we just want to make sure it
  // compiles.
  if (fields.size() == 5) {
    std::cout << "OK" << std::endl << std::endl;
  }
}
}  // namespace test_meta_fields
