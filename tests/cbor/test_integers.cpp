#include <gtest/gtest.h>

#include <rfl/cbor.hpp>

namespace test_integers {

TEST(cbor, test_integers_signedness) {

    static constexpr uint64_t BIG_INT = 0xffffffffffffffff;

    struct Unsigned {
        uint64_t u64;
    };

    struct Signed {
        int64_t i64;
    };

    std::vector<char> unsigned_buffer = rfl::cbor::write(Unsigned{BIG_INT});
    std::vector<unsigned char> unsigned_expected = {
        0xBF, 0x63, 0x75, 0x36, 0x34,
        0x1B, // Per RFC 8949, Initial byte '0x1B' indicates "unsigned integer (eight-byte uint64_t follows)"
              // See: https://www.rfc-editor.org/rfc/rfc8949.html#section-appendix.b
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF
    };

    EXPECT_EQ(std::vector<char>(unsigned_expected.begin(), unsigned_expected.end()), unsigned_buffer);

    std::vector<char> signed_buffer = rfl::cbor::write(Signed{static_cast<int64_t>(BIG_INT)});
    std::vector<unsigned char> signed_expected = {
        0xBF, 0x63, 0x69, 0x36, 0x34,
        0x20, // Per RFC 8949, Initial byte '0x20' indicates "negative integer -1-0x00..-1-0x17 (-1..-24)"
              // See: https://www.rfc-editor.org/rfc/rfc8949.html#section-appendix.b
        0xFF
    };

    EXPECT_EQ(std::vector<char>(signed_expected.begin(), signed_expected.end()), signed_buffer);
}

}  // namespace test_integers
