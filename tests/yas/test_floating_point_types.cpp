#include <gtest/gtest.h>

#include <rfl/json.hpp>
#include <rfl/yas.hpp>

namespace test_floating_point_types {

struct FloatAndDoubleStruct {
  float f;
  double d;
};

TEST(yas, writes_float_and_double) {
  const FloatAndDoubleStruct s{1.0f, 2.0};

  const auto bytes = rfl::yas::write(s);
  const auto res = rfl::yas::read<FloatAndDoubleStruct>(bytes);

  EXPECT_TRUE(res && true) << "Error: " << res.error().what();

  EXPECT_EQ(res.value().f, s.f);
  EXPECT_EQ(res.value().d, s.d);

  const auto json1 = rfl::json::write(s);
  const auto json2 = rfl::json::write(res.value());
  EXPECT_EQ(json1, json2);
}

}  // namespace test_floating_point_types
