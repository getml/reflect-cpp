#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_add_tag_to_rfl_variant {

struct button_pressed_t {};

// Test that we can still use structs that must be moved
struct button_released_t {
  rfl::Box<int> button;
};

struct key_pressed_t {
  using Tag = rfl::Literal<"key_pressed">;
  char key;
};

using my_event_type_t =
    rfl::Variant<button_pressed_t, button_released_t, key_pressed_t, int>;

TEST(json, test_add_tag_to_rfl_variant) {
  std::vector<my_event_type_t> vec;
  vec.emplace_back(button_pressed_t{});
  vec.emplace_back(button_released_t{rfl::make_box<int>(4)});
  vec.emplace_back(key_pressed_t{'c'});
  vec.emplace_back(3);

  write_and_read<rfl::AddTagsToVariants>(
      vec,
      R"([{"test_add_tag_to_rfl_variant::button_pressed_t":{}},{"test_add_tag_to_rfl_variant::button_released_t":{"button":4}},{"key_pressed":{"key":99}},{"int":3}])");
}
}  // namespace test_add_tag_to_rfl_variant
