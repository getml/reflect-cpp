#include <rfl/cereal.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(cereal, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::cereal::write(value); },
              [](const auto& data) { return rfl::cereal::read<Data>(data); });
}

}  // namespace test_vector_reserve
