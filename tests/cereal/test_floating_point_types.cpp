#include <gtest/gtest.h>

#include <rfl/cereal.hpp>

namespace test_floating_point_types {

struct FloatAndDoubleStruct {
  float f;
  double d;
};

TEST(cereal, writes_float_and_double) {
  const FloatAndDoubleStruct s{1.0f, 2.0};

  const auto bytes = rfl::cereal::write(s);
  const auto res = rfl::cereal::read<FloatAndDoubleStruct>(bytes);

  EXPECT_TRUE(res && true) << "Error: " << res.error().what();

  EXPECT_EQ(res.value().f, s.f);
  EXPECT_EQ(res.value().d, s.d);
}

}  // namespace test_floating_point_types
