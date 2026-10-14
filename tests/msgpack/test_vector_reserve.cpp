#include <rfl/msgpack.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(msgpack, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::msgpack::write(value); },
              [](const auto& data) { return rfl::msgpack::read<Data>(data); });
}

}  // namespace test_vector_reserve
