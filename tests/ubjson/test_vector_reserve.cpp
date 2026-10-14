#include <rfl/ubjson.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(ubjson, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::ubjson::write(value); },
              [](const auto& data) { return rfl::ubjson::read<Data>(data); });
}

}  // namespace test_vector_reserve
