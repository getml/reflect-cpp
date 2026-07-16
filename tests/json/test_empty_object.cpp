#include <string>

#include "write_and_read.hpp"
import rfl;
import rfl.json;


namespace test_empty_object {

struct Empty {};

TEST(json, test_empty_object) {
  const auto empty = Empty{};

  write_and_read(empty, R"({})");
}
}  // namespace test_empty_object
