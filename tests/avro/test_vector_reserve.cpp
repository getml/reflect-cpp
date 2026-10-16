#include <rfl/avro.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(avro, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::avro::write(value); },
              [](const auto& data) { return rfl::avro::read<Data>(data); });
}

}  // namespace test_vector_reserve
