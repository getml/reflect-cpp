#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.cereal.hpp>


namespace test_tuple {

TEST(cereal, test_tuple) {
  const auto my_tuple = std::make_tuple(42, std::string("Hello"), true, 3.14);
  write_and_read(my_tuple);
}

}  // namespace test_tuple
