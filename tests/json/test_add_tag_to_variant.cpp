#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <variant>
#include <vector>

#include "write_and_read.hpp"

namespace test_add_tag_to_variant {

// test 1 -> normal behaviour

struct button_pressed_t {};

struct button_released_t {};

struct key_pressed_t {
  using Tag = rfl::Literal<"key_pressed">;
  char key;
};

using my_event_type_t =
    std::variant<button_pressed_t, button_released_t, key_pressed_t, int>;

// test 2 -> 'Generic' within a struct like this cannot be read/written
// due to the underlying `std::variant` holding two fields with name 'Generic'
// once 'remove_namespaces' is applied to the type name extraction (which is removed)
// in the MR this test is added
struct APIResult {
  rfl::Generic result;
};
struct APIError {
  rfl::Generic error;
};

using APICallOutput = rfl::Variant<APIResult, APIError>;

// test 3 -> two structs with the same name in different namespaces should still
// be serializable

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

using Messages = std::variant<Result::Message, Error::Message>;

TEST(json, test_add_tag_to_variant) {
  const auto vec = std::vector<my_event_type_t>(
      {button_pressed_t{}, button_released_t{}, key_pressed_t{'c'}, 3});

  write_and_read<rfl::AddTagsToVariants>(
      vec,
      R"([{"test_add_tag_to_variant::button_pressed_t":{}},{"test_add_tag_to_variant::button_released_t":{}},{"key_pressed":{"key":99}},{"int":3}])");
}

TEST(json, test_add_tag_to_variant_with_generic) {
  APICallOutput output = APIResult{"200"};
  write_and_read<rfl::AddTagsToVariants>(output,
    R"({"test_add_tag_to_variant::APIResult":{"result":{"std::string":"200"}}})");
}
TEST(json, test_add_tag_to_variant_different_namespaces) {
  Messages m = Error::Message{.error="an error", .error_id=2};
  write_and_read<rfl::AddTagsToVariants>(m,
    R"({"test_add_tag_to_variant::Error::Message":{"error":"an error","error_id":2}})");
}
}  // namespace test_add_tag_to_variant
