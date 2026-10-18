#include <rfl/flexbuf.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(flexbuffers, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::flexbuf::write(value); },
              [](const auto& data) { return rfl::flexbuf::read<Data>(data); });
}

}  // namespace test_vector_reserve
