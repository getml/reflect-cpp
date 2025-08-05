#include <gtest/gtest.h>

#include <rfl/json.hpp>

namespace test_enum_underlying {

enum class CameraSource : uint32_t { kWide, kInfrared, kZoom };

struct ChangeCameraSourcePayload {
  CameraSource source;
};

constexpr const char* payloads[2] = {R"({"source":20})",
                                     R"({"source":"hello"})"};

TEST(json, test_enum_underlying) {
  const auto res1 = static_cast<int>(
      rfl::json::read<ChangeCameraSourcePayload, rfl::UnderlyingEnums>(
          payloads[0])
          .value()
          .source);
  const auto res2 =
      rfl::json::read<CameraSource, rfl::UnderlyingEnums>(payloads[1]);

  EXPECT_EQ(res1, 20);
  EXPECT_TRUE(!res2);  // should not succeed
}

}  // namespace test_enum_underlying
