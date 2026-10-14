#include <rfl/capnproto.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(capnproto, test_vector_reserve) {
  check_moves(
      [](const Data& value) { return rfl::capnproto::write(value); },
      [](const auto& data) { return rfl::capnproto::read<Data>(data); });
}

}  // namespace test_vector_reserve
