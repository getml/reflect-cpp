#include <capnp/dynamic.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <test_rfl/rfl.capnproto.hpp>

namespace test_capnproto_native_constructor {
struct Record {
  std::string text;
  int64_t count;
  static Record from_capnproto_obj(const auto& _var) {
    const auto object = _var.val_.template as<capnp::DynamicStruct>();
    return {
        std::string(object.get("text").template as<capnp::Text>().cStr()) + "!",
        object.get("count").template as<int64_t>()};
  }
};
TEST(capnproto, native_constructor) {
  const auto parsed = rfl::capnproto::read<Record>(
      rfl::capnproto::write(Record{"native", -42}));
  ASSERT_TRUE(parsed);
  EXPECT_EQ(parsed->text, "native!");
  EXPECT_EQ(parsed->count, -42);
}
}  // namespace test_capnproto_native_constructor
