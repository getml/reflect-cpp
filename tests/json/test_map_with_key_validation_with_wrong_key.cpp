#include <map>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_map_with_key_validation_with_wrong_key {

using Between1and3 = rfl::Validator<int, rfl::Minimum<1>, rfl::Maximum<3>>;
using MapT = std::map<Between1and3, std::string>;

TEST(json, test_map_with_key_validation_with_wrong_key) {
  const auto res = rfl::json::read<MapT>(R"({"0":"zero"})");
  EXPECT_FALSE(res);
}

}  // namespace test_map_with_key_validation_with_wrong_key
