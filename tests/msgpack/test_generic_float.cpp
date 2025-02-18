#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_generic_float {

TEST(msgpack, test_generic_float) {
  rfl::Generic msgpack_data_before{};
  rfl::Generic msgpack_data_after{};

  msgpack_data_before = 1.4F;

  auto tmp_msgpack =
      rfl::msgpack::write<rfl::UnderlyingEnums>(msgpack_data_before);
  msgpack_data_after = *rfl::msgpack::read<rfl::Generic>(tmp_msgpack);

  EXPECT_EQ(msgpack_data_before.to_double().value(),
            msgpack_data_after.to_double().value());
}
}  // namespace test_generic_float
