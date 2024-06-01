#include "test_oid.hpp"

#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_oid {

struct Person {
  std::string first_name;
  std::string last_name;
  std::vector<bson_oid_t> oids;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  auto oids = std::vector<bson_oid_t>(3);

  for (auto& oid : oids) {
    bson_oid_init(&oid, NULL);
  }

  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .oids = oids};

  write_and_read(homer);
}
}  // namespace test_oid
