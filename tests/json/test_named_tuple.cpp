#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_named_tuple {

TEST(json, test_named_tuple) {
  const auto homer = rfl::make_field<"firstName", std::string>("Homer") *
                     rfl::make_field<"lastName", std::string>("Simpson") *
                     rfl::make_field<"age">(45);

  write_and_read(homer,
                 R"({"firstName":"Homer","lastName":"Simpson","age":45})");
}
}  // namespace test_named_tuple
