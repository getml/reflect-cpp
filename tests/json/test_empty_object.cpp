#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


namespace test_empty_object {

struct Empty {};

TEST(json, test_empty_object) {
  const auto empty = Empty{};

  write_and_read(empty, R"({})");
}
}  // namespace test_empty_object
