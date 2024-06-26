#include <iostream>
#include <rfl.hpp>
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
#ifdef REFLECT_CPP_C_ARRAYS_OR_INHERITANCE
  auto oids = std::vector<bson_oid_t>(3);

  for (auto& oid : oids) {
    bson_oid_init(&oid, NULL);
  }

  const auto homer =
      Person{.first_name = "Homer", .last_name = "Simpson", .oids = oids};

  write_and_read(homer);
#endif
}
}  // namespace test_oid
