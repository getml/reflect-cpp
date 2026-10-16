#include <limits>
#include <rfl/json.hpp>
#include <rfl/parsing/VectorReader.hpp>

#include "../check_vector_reserve.hpp"

namespace test_vector_reserve {

TEST(json, test_vector_reserve) {
  check_moves([](const Data& value) { return rfl::json::write(value); },
              [](const auto& data) { return rfl::json::read<Data>(data); });
}

TEST(json, test_vector_reserve_excessive_size) {
  std::vector<float> values;
  const rfl::json::Reader reader;
  const rfl::parsing::VectorReader<rfl::json::Reader, rfl::json::Writer,
                                   std::vector<float>, rfl::Processors<>>
      vector_reader(&reader, &values);
  const auto error =
      vector_reader.reserve(std::numeric_limits<std::size_t>::max());
  EXPECT_TRUE(error.has_value());
  EXPECT_TRUE(values.empty());
}

}  // namespace test_vector_reserve
