#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_double {

TEST(yaml, test_double) {
  auto obj = rfl::Generic::Object();
  const double age = 10;
  obj["age"] = age;
  const auto s = rfl::yaml::write(obj);
  EXPECT_EQ(
      rfl::from_generic<double>(
          rfl::yaml::read<rfl::Generic::Object>(s).value().get("age").value())
          .value(),
      10.0);
}
}  // namespace test_double
