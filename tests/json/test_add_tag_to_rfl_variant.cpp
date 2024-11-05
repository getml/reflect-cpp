#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <variant>
#include <vector>

#include "write_and_read.hpp"

namespace test_add_tag_to_rfl_variant {

struct button_pressed_t {};

struct button_released_t {};

struct key_pressed_t {
  using Tag = rfl::Literal<"key_pressed">;
  char key;
};

using my_event_type_t =
    rfl::Variant<button_pressed_t, button_released_t, key_pressed_t, int>;

TEST(json, test_add_tag_to_rfl_variant) {
  const auto vec = std::vector<my_event_type_t>(
      {button_pressed_t{}, button_released_t{}, key_pressed_t{'c'}, 3});

  write_and_read<rfl::AddTagsToVariants>(
      vec,
      R"([{"button_pressed_t":{}},{"button_released_t":{}},{"key_pressed":{"key":99}},{"int":3}])");
}
}  // namespace test_add_tag_to_rfl_variant
