#include <rfl/cbor.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(cbor, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::cbor::write(value); },
              [](const auto& data) { return rfl::cbor::read<Data>(data); });
}

}  // namespace test_vector_reserve
