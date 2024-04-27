#include <cstdint>
#include <rfl.hpp>
#include <rfl/json.hpp>

#include "write_and_read.hpp"

namespace test_integers {

TEST(json, test_integers) {

  struct Integers {
      int i32;
      unsigned int u32;
      long long i64;
      unsigned long long u64;
  };

  write_and_read(Integers{.i32 = INT32_MAX, .u32 = UINT32_MAX, .i64 = INT64_MAX, .u64 = UINT64_MAX}, R"({"i32":2147483647,"u32":4294967295,"i64":9223372036854775807,"u64":18446744073709551615})");
  write_and_read(Integers{.i32 = INT32_MIN, .u32 = 0, .i64 = INT64_MIN, .u64 = 0}, R"({"i32":-2147483648,"u32":0,"i64":-9223372036854775808,"u64":0})");
}

}  // namespace test_integers
