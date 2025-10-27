#include <cassert>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// NOTE TO MAINTAINERS:
// These tests are very similar to the others in
// test_add_*tags_to_*variants.cpp, so please keep them in sync.
namespace test_variant_auto_tagging {

// test 1 -> normal behaviour

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

TEST(cbor, test_add_tags_to_rfl_variants) {
  std::vector<my_event_type_t> vec;
  vec.emplace_back(button_pressed_t{});
  vec.emplace_back(button_released_t{rfl::make_box<int>(4)});
  vec.emplace_back(key_pressed_t{'c'});
  vec.emplace_back(3);
  write_and_read<rfl::AddTagsToVariants>(
      vec,
      // Diagnostic notation: [{_ "button_pressed_t": {_ }}, {_
      // "button_released_t": {_ "button": 4}}, {_ "key_pressed": {_ "key":
      // 99}}, {_ "int": 3}] See:
      // https://cbor.me/?diag=[{_%20%22button_pressed_t%22:%20{_%20}},%20{_%20%22button_released_t%22:%20{_%20%22button%22:%204}},%20{_%20%22key_pressed%22:%20{_%20%22key%22:%2099}},%20{_%20%22int%22:%203}]
      R"(
84                                      # array(4)
   BF                                   # map(*)
      70                                # text(16)
         627574746F6E5F707265737365645F74 # "button_pressed_t"
      BF                                # map(*)
         FF                             # primitive(*)
      FF                                # primitive(*)
   BF                                   # map(*)
      71                                # text(17)
         627574746F6E5F72656C65617365645F74 # "button_released_t"
      BF                                # map(*)
         66                             # text(6)
            627574746F6E                # "button"
         04                             # unsigned(4)
         FF                             # primitive(*)
      FF                                # primitive(*)
   BF                                   # map(*)
      6B                                # text(11)
         6B65795F70726573736564         # "key_pressed"
      BF                                # map(*)
         63                             # text(3)
            6B6579                      # "key"
         18 63                          # unsigned(99)
         FF                             # primitive(*)
      FF                                # primitive(*)
   BF                                   # map(*)
      63                                # text(3)
         696E74                         # "int"
      03                                # unsigned(3)
      FF                                # primitive(*)
    )");
}

// test 2 -> 'Generic' within a struct like this can be read/written with
// rfl::AddNamespacedTagsToVariants so that the underlying `std::variant` holds
// two 'Generic' fields with different namespaces. See the similar test in
// test_add_tags_to_variants.cpp. A hypothetical alternative would be adding an
// optional manual tag to rfl::Generic

struct APIResult {
  rfl::Generic result;
};
struct APIError {
  rfl::Generic error;
};

using APICallOutput = std::variant<APIResult, APIError>;

TEST(cbor, test_add_namespaced_tags_to_variants_with_generic) {
  std::vector<APICallOutput> output = {APIResult{"200"}, APIError{"an error"}};
  write_and_read<rfl::AddNamespacedTagsToVariants>(
      output,
      // Diagnostic notation: [{_ "test_variant_auto_tagging::APIResult": {_
      // "result": {_ "std::string": "200"}}}, {_
      // "test_variant_auto_tagging::APIError": {_ "error": {_ "std::string":
      // "an error"}}}] See:
      // https://cbor.me/?diag=[{_%20%22test_variant_auto_tagging::APIResult%22:%20{_%20%22result%22:%20{_%20%22std::string%22:%20%22200%22}}},%20{_%20%22test_variant_auto_tagging::APIError%22:%20{_%20%22error%22:%20{_%20%22std::string%22:%20%22an%20error%22}}}]
      R"(
82                                      # array(2)
   BF                                   # map(*)
      78 24                             # text(36)
         746573745F76617269616E745F6175746F5F74616767696E673A3A415049526573756C74 # "test_variant_auto_tagging::APIResult"
      BF                                # map(*)
         66                             # text(6)
            726573756C74                # "result"
         BF                             # map(*)
            6B                          # text(11)
               7374643A3A737472696E67   # "std::string"
            63                          # text(3)
               323030                   # "200"
            FF                          # primitive(*)
         FF                             # primitive(*)
      FF                                # primitive(*)
   BF                                   # map(*)
      78 23                             # text(35)
         746573745F76617269616E745F6175746F5F74616767696E673A3A4150494572726F72 # "test_variant_auto_tagging::APIError"
      BF                                # map(*)
         65                             # text(5)
            6572726F72                  # "error"
         BF                             # map(*)
            6B                          # text(11)
               7374643A3A737472696E67   # "std::string"
            68                          # text(8)
               616E206572726F72         # "an error"
            FF                          # primitive(*)
         FF                             # primitive(*)
      FF                                # primitive(*)
    )");
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

using Messages = std::variant<Result::Message, Error::Message>;

TEST(cbor, test_add_namespaced_tags_to_variants_different_namespaces) {
  std::vector<Messages> msgs{
      Result::Message{.result = "a result"},
      Error::Message{.error = "an error", .error_id = 2},
  };
  write_and_read<rfl::AddNamespacedTagsToVariants>(
      msgs,
      // Diagnostic notation: [{_
      // "test_variant_auto_tagging::Result::Message":{_ "result":"a
      // result"}},{_ "test_variant_auto_tagging::Error::Message":{_ "error":"an
      // error", "error_id":2}}] See:
      // https://cbor.me/?diag=[{_%20%22test_variant_auto_tagging::Result::Message%22:{_%20%22result%22:%22a%20result%22}},{_%20%22test_variant_auto_tagging::Error::Message%22:{_%20%22error%22:%22an%20error%22,%20%22error_id%22:2}}]
      R"(
82                                      # array(2)
   BF                                   # map(*)
      78 2A                             # text(42)
         746573745F76617269616E745F6175746F5F74616767696E673A3A526573756C743A3A4D657373616765 # "test_variant_auto_tagging::Result::Message"
      BF                                # map(*)
         66                             # text(6)
            726573756C74                # "result"
         68                             # text(8)
            6120726573756C74            # "a result"
         FF                             # primitive(*)
      FF                                # primitive(*)
   BF                                   # map(*)
      78 29                             # text(41)
         746573745F76617269616E745F6175746F5F74616767696E673A3A4572726F723A3A4D657373616765 # "test_variant_auto_tagging::Error::Message"
      BF                                # map(*)
         65                             # text(5)
            6572726F72                  # "error"
         68                             # text(8)
            616E206572726F72            # "an error"
         68                             # text(8)
            6572726F725F6964            # "error_id"
         02                             # unsigned(2)
         FF                             # primitive(*)
      FF                                # primitive(*)
    )");
}

}  // namespace test_variant_auto_tagging
