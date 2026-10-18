#ifndef RFL_TEST_PARQUET_NATIVE_REFERENCE_HPP_
#define RFL_TEST_PARQUET_NATIVE_REFERENCE_HPP_
#include <vector>
namespace test_parquet_bridge {
std::vector<char> reference_bytes();
bool reference_check(const std::vector<char>&);
}  // namespace test_parquet_bridge
#endif
