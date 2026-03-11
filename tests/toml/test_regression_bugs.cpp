#include <gtest/gtest.h>

#include <string>
#include <vector>

#include <rfl.hpp>
#include <rfl/toml.hpp>

// toml::Writer — at_path() may return invalid node, as_table()/as_array()
// returns nullptr
// File: src/rfl/toml/Writer.cpp:26, :40
// After emplace(_name, ...), at_path(_name) is used to find the just-inserted
// value. But at_path() interprets '.' as a path separator. So a field named
// "a.b" is inserted as a literal key "a.b", but at_path("a.b") looks for
// nested path a→b → returns invalid node → as_table()/as_array() = nullptr.
namespace test_toml_writer_at_path_nullptr {

// Use rfl::Rename to create a field name with a dot, triggering the bug.
struct Inner {
  int x = 1;
};

struct WithDottedKey {
  rfl::Rename<"a.b", Inner> dotted_field;
};

TEST(toml_regression, field_name_with_dot_does_not_crash) {
  const auto obj = WithDottedKey{.dotted_field = Inner{.x = 42}};
  // With the bug: emplace inserts key "a.b", but at_path("a.b") interprets
  // the dot as path separator and looks for table["a"]["b"] which doesn't
  // exist → returns invalid node → as_table() returns nullptr → crash.
  // After fix: should handle dotted keys correctly.
  const auto serialized = rfl::toml::write(obj);
  const auto res = rfl::toml::read<WithDottedKey>(
      std::string_view(serialized.c_str(), serialized.size()));
  ASSERT_TRUE(res) << "TOML roundtrip with dotted key failed: "
                   << res.error().what();
  EXPECT_EQ(res.value().dotted_field.get().x, 42);
}

struct WithDottedArray {
  rfl::Rename<"a.b", std::vector<int>> dotted_array;
};

TEST(toml_regression, array_field_name_with_dot_does_not_crash) {
  const auto obj = WithDottedArray{.dotted_array = std::vector<int>{1, 2, 3}};
  // Same bug: add_array_to_object uses at_path("a.b") → nullptr.
  const auto serialized = rfl::toml::write(obj);
  const auto res = rfl::toml::read<WithDottedArray>(
      std::string_view(serialized.c_str(), serialized.size()));
  ASSERT_TRUE(res) << "TOML roundtrip with dotted array key failed: "
                   << res.error().what();
  ASSERT_EQ(res.value().dotted_array.get().size(), 3u);
  EXPECT_EQ(res.value().dotted_array.get()[0], 1);
}

}  // namespace test_toml_writer_at_path_nullptr
