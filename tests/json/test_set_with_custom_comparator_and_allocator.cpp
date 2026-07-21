#include <rfl.hpp>
#include <rfl/json.hpp>
#include <set>
#include <string>

#include "write_and_read.hpp"

namespace test_set_with_custom_comparator_and_allocator {

struct CustomLess {
  bool operator()(const int& a, const int& b) const { return a < b; }
};

struct Person {
  rfl::Rename<"firstName", std::string> first_name;
  rfl::Rename<"lastName", std::string> last_name = "Simpson";
  std::unique_ptr<std::set<int, CustomLess, std::allocator<int>>> ages;
};

TEST(json, test_set_with_custom_comparator_and_allocator) {
  auto ages = std::make_unique<std::set<int, CustomLess, std::allocator<int>>>(
      std::set<int, CustomLess, std::allocator<int>>({20, 30, 40}));

  const auto homer =
      Person{.first_name = "Homer", .ages = std::move(ages)};

  write_and_read(
      homer,
      R"({"firstName":"Homer","lastName":"Simpson","ages":[20,30,40]})");
}
}  // namespace test_set_with_custom_comparator_and_allocator
