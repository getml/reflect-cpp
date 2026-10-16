#include <rfl/yas.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(yas, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::yas::write(value); },
              [](const auto& data) { return rfl::yas::read<Data>(data); });
}

}  // namespace test_vector_reserve
