#include <rfl/boost_serialization.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(boost_serialization, test_vector_reserve) {
  check_moves(
      [](const Data& value) { return rfl::boost_serialization::write(value); },
      [](const auto& data) {
        return rfl::boost_serialization::read<Data>(data);
      });
}

}  // namespace test_vector_reserve
