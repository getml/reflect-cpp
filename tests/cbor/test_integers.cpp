#include <gtest/gtest.h>

#include <rfl/cbor.hpp>
#include "write_and_read.hpp"

namespace test_integers {

TEST(cbor, test_integers_signedness) {

    static constexpr uint64_t BIG_INT = 0xffffffffffffffff;

    struct Unsigned {
        uint64_t u64;
    };

    struct Signed {
        int64_t i64;
    };

    write_and_read(
        Unsigned{BIG_INT},
        // From https://cbor.me/?diag={_%20%22u64%22:%200xffffffffffffffff}
        R"(
BF                     # map(*)
   63                  # text(3)
      753634           # "u64"
   1B FFFFFFFFFFFFFFFF # unsigned(18446744073709551615)
   FF                  # primitive(*)
    )");

    write_and_read(
        Signed{static_cast<int64_t>(BIG_INT)},
        // From https://cbor.me/?diag={_%20%22i64%22:%20-1}
        R"(
BF           # map(*)
   63        # text(3)
      693634 # "i64"
   20        # negative(0)
   FF        # primitive(*)
    )");
}

}  // namespace test_integers
