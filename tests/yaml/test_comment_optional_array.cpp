#include <gtest/gtest.h>

#include <array>
#include <optional>
#include <rfl/Commented.hpp>
#include <rfl/yaml.hpp>

#include "write_and_read.hpp"

namespace test_comment_optional_array {

struct VeryCoolYaml {
  rfl::Commented<std::optional<std::array<unsigned, 3>>> optionalArray =
      std::nullopt;
};

TEST(yaml, test_comment_optional_array) {
  const auto yaml = VeryCoolYaml{};
  write_and_read(yaml);
}

}  // namespace test_comment_optional_array
