#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_object {

TEST(json, test_object) {
  rfl::Object<rfl::Generic> o;
  o["hello"] = "world";
  EXPECT_EQ(o.get("hello").value().to_string().value(), "world");
  EXPECT_EQ(o.at("hello").to_string().value(), "world");
}

}  // namespace test_object
