#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <unordered_set>

#include "write_and_read.hpp"

namespace test_unordered_set_with_custom_hash_and_allocator {

template <typename T>
struct DummyHash {
  std::size_t operator()(const T& v) const { return std::hash<T>()(v); }
};

template <typename T>
struct DummyKeyEqual {
  bool operator()(const T& a, const T& b) const { return a == b; }
};

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<
      std::unordered_set<int, DummyHash<int>, DummyKeyEqual<int>,
                         std::allocator<int>>>
      ages;
};

TEST(json, test_unordered_set_with_custom_hash_and_allocator) {
  auto ages = std::make_unique<
      std::unordered_set<int, DummyHash<int>, DummyKeyEqual<int>,
                         std::allocator<int>>>(
      std::unordered_set<int, DummyHash<int>, DummyKeyEqual<int>,
                         std::allocator<int>>({20, 30, 40}));

  const auto homer =
      Person{.first_name = "Homer", .ages = std::move(ages)};

  // Unordered sets are unpredictable. We therefore only make sure that this
  // compiles.
  const auto json_string = rfl::json::write(homer);
  const auto homer2 = rfl::json::read<Person>(json_string);
  EXPECT_TRUE(homer2 && true);
}
}  // namespace test_unordered_set_with_custom_hash_and_allocator
