#ifndef CHECK_NO_FIELD_NAMES_DISPATCH_HPP_
#define CHECK_NO_FIELD_NAMES_DISPATCH_HPP_
#include <gtest/gtest.h>

#include <rfl.hpp>
#include <string>
#include <vector>

namespace test_no_field_names_dispatch {
struct Record {
  int first;
  std::string middle;
  int last;
  bool operator==(const Record&) const = default;
};
struct Data {
  std::vector<Record> records;
  bool operator==(const Data&) const = default;
};
template <class Write, class Read>
void check_roundtrip(Write write, Read read) {
  const Data input{{{1, "first", 2}, {-3, "second", 4}}};
  const auto bytes = write(input);
  const auto result = read.template operator()<Data>(bytes);
  ASSERT_TRUE(result) << result.error().what();
  EXPECT_EQ(*result, input);
  EXPECT_EQ(write(*result), bytes);
}
}  // namespace test_no_field_names_dispatch
#endif
