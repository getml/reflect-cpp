#include <rfl/yaml.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(yaml, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::yaml::write(value); },
              [](const auto& data) { return rfl::yaml::read<Data>(data); });
}

}  // namespace test_vector_reserve
