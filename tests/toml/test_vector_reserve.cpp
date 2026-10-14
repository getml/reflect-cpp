#include <rfl/toml.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(toml, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::toml::write(value); },
              [](const auto& data) { return rfl::toml::read<Data>(data); });
}

}  // namespace test_vector_reserve
