#include <cstdint>
#include <iostream>
#include <optional>
#include <rfl.hpp>
#include <rfl/json.hpp>

// Keep both types in an anonymous namespace: external linkage hides the
// GCC 12.5 -O2 regression from issue #719.
namespace {

struct Inner {
  std::optional<std::uint32_t> a;
  std::optional<int> b;
};

struct Outer {
  std::optional<std::uint32_t> f1;
  std::optional<std::uint32_t> f2;
  std::optional<std::uint32_t> f3;
  std::optional<std::uint32_t> f4;
  std::optional<std::uint32_t> f5;
  std::optional<std::uint32_t> f6;
  std::optional<std::uint32_t> f7;
  std::optional<std::uint32_t> f8;
  std::optional<std::uint32_t> f9;
  std::optional<std::uint32_t> f10;
  std::optional<std::uint32_t> f11;
  std::optional<Inner> inner;
};

}  // namespace

int main() {
  const auto res = rfl::json::read<Outer>("{}");
  std::cout << (res ? "OK" : "parse error") << std::endl;
  return res ? 0 : 1;
}
