#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// NOTE TO MAINTAINERS:
// These tests are very similar to the others in test_add_*tags_to_*variants.cpp, so please keep them
// in sync.
namespace test_add_namespaced_tags_to_rfl_variants {

// test 1 -> normal behaviour

struct button_pressed_t {};

// Test that we can still use structs that must be moved
struct button_released_t {
  rfl::Box<int> button;
};

struct key_pressed_t {
  // Manually specified tags don't get the namespace prefixed
  using Tag = rfl::Literal<"key_pressed">;
  char key;
};

using my_event_type_t =
    rfl::Variant<button_pressed_t, button_released_t, key_pressed_t, int>;

TEST(json, test_add_namespaced_tags_to_rfl_variants) {
  std::vector<my_event_type_t> vec;
  vec.emplace_back(button_pressed_t{});
  vec.emplace_back(button_released_t{rfl::make_box<int>(4)});
  vec.emplace_back(key_pressed_t{'c'});
  vec.emplace_back(3);

  write_and_read<rfl::AddNamespacedTagsToVariants>(
      vec,
      R"([{"test_add_namespaced_tags_to_rfl_variants::button_pressed_t":{}},{"test_add_namespaced_tags_to_rfl_variants::button_released_t":{"button":4}},{"key_pressed":{"key":99}},{"int":3}])");
}

// test 2 -> 'Generic' within a struct like this can be read/written with
// rfl::AddNamespacedTagsToVariants so that the underlying `std::variant` holds two 'Generic' fields
// with different namespaces. See the similar test in test_add_tags_to_variants.cpp.
// A hypothetical alternative would be adding an optional manual tag to rfl::Generic

struct APIResult {
  rfl::Generic result;
};
struct APIError {
  rfl::Generic error;
};

using APICallOutput = rfl::Variant<APIResult, APIError>;

TEST(json, test_add_namespaced_tags_to_rfl_variants_with_generic) {
  APICallOutput output = APIResult{"200"};
  write_and_read<rfl::AddNamespacedTagsToVariants>(output,
    R"({"test_add_namespaced_tags_to_rfl_variants::APIResult":{"result":{"std::string":"200"}}})");
}

// test 3 -> two structs with the same name in different namespaces should still
// be serializable because we're using rfl::AddNamespacedTagsToVariants

namespace Result {
struct Message {
  std::string result;
};
}  // namespace Result

namespace Error {
struct Message {
  std::string error;
  int error_id;
};
};  // namespace Error

using Messages = rfl::Variant<Result::Message, Error::Message>;

TEST(json, test_add_namespaced_tags_to_rfl_variants_different_namespaces) {
  std::vector<Messages> msgs{
    Result::Message{.result="a result"},
    Error::Message{.error="an error", .error_id=2},
  };
  write_and_read<rfl::AddNamespacedTagsToVariants>(msgs,
    R"([{"test_add_namespaced_tags_to_rfl_variants::Result::Message":{"result":"a result"}},{"test_add_namespaced_tags_to_rfl_variants::Error::Message":{"error":"an error","error_id":2}}])");
}
}  // namespace test_add_namespaced_tags_to_rfl_variants
