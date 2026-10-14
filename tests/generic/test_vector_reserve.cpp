#include <rfl.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(generic, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::to_generic(value); },
              [](const auto& data) { return rfl::from_generic<Data>(data); });
}

}  // namespace test_vector_reserve
