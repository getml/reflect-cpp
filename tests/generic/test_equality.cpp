#include <gtest/gtest.h>

#include <cstdint>
#include <string>

#include <rfl.hpp>

namespace test_generic_equality {

TEST(generic, equality_of_scalars) {
  EXPECT_TRUE(rfl::Generic(int64_t{42}) == rfl::Generic(int64_t{42}));
  EXPECT_FALSE(rfl::Generic(int64_t{42}) == rfl::Generic(int64_t{43}));
  EXPECT_TRUE(rfl::Generic(std::string("abc")) ==
              rfl::Generic(std::string("abc")));
  EXPECT_FALSE(rfl::Generic(std::string("abc")) ==
               rfl::Generic(std::string("abd")));
  EXPECT_TRUE(rfl::Generic(true) == rfl::Generic(true));
  EXPECT_FALSE(rfl::Generic(true) == rfl::Generic(false));
  EXPECT_TRUE(rfl::Generic(3.14) == rfl::Generic(3.14));

  // operator!= is synthesized from operator== in C++20.
  EXPECT_TRUE(rfl::Generic(int64_t{42}) != rfl::Generic(int64_t{43}));
  EXPECT_FALSE(rfl::Generic(int64_t{42}) != rfl::Generic(int64_t{42}));
}

TEST(generic, equality_of_different_alternatives) {
  EXPECT_FALSE(rfl::Generic(int64_t{1}) == rfl::Generic(std::string("1")));
  EXPECT_TRUE(rfl::Generic(int64_t{1}) != rfl::Generic(1.0));
}

TEST(generic, equality_of_null) {
  const auto null1 = rfl::Generic();
  const auto null2 = rfl::Generic();
  EXPECT_TRUE(null1 == null2);
  EXPECT_TRUE(null1 != rfl::Generic(int64_t{0}));
}

TEST(generic, equality_of_objects) {
  rfl::Object<rfl::Generic> obj1;
  obj1["a"] = rfl::Generic(int64_t{1});
  obj1["b"] = rfl::Generic(std::string("x"));

  rfl::Object<rfl::Generic> obj2;
  obj2["a"] = rfl::Generic(int64_t{1});
  obj2["b"] = rfl::Generic(std::string("x"));

  EXPECT_TRUE(obj1 == obj2);
}

TEST(generic, equality_of_objects_is_order_dependent) {
  // rfl::Object preserves insertion order and equality is positional, so
  // objects with the same pairs in a different order are not equal.
  rfl::Object<rfl::Generic> obj1;
  obj1["a"] = rfl::Generic(int64_t{1});
  obj1["b"] = rfl::Generic(std::string("x"));

  rfl::Object<rfl::Generic> reordered;
  reordered["b"] = rfl::Generic(std::string("x"));
  reordered["a"] = rfl::Generic(int64_t{1});

  EXPECT_FALSE(obj1 == reordered);
}

TEST(generic, equality_of_objects_with_duplicate_keys) {
  // Duplicate keys are a supported state (e.g. produced by json::read).
  // Positional comparison handles them correctly.
  const std::string key = "k";

  rfl::Object<rfl::Generic> obj1;
  obj1.insert(key, rfl::Generic(int64_t{1}));
  obj1.insert(key, rfl::Generic(int64_t{2}));

  rfl::Object<rfl::Generic> same;
  same.insert(key, rfl::Generic(int64_t{1}));
  same.insert(key, rfl::Generic(int64_t{2}));
  EXPECT_TRUE(obj1 == same);

  rfl::Object<rfl::Generic> swapped;
  swapped.insert(key, rfl::Generic(int64_t{2}));
  swapped.insert(key, rfl::Generic(int64_t{1}));
  EXPECT_FALSE(obj1 == swapped);
}

TEST(generic, inequality_of_objects) {
  rfl::Object<rfl::Generic> obj1;
  obj1["a"] = rfl::Generic(int64_t{1});
  obj1["b"] = rfl::Generic(std::string("x"));

  rfl::Object<rfl::Generic> different_value;
  different_value["a"] = rfl::Generic(int64_t{1});
  different_value["b"] = rfl::Generic(std::string("y"));
  EXPECT_TRUE(obj1 != different_value);

  rfl::Object<rfl::Generic> different_size;
  different_size["a"] = rfl::Generic(int64_t{1});
  EXPECT_TRUE(obj1 != different_size);

  rfl::Object<rfl::Generic> different_key;
  different_key["a"] = rfl::Generic(int64_t{1});
  different_key["c"] = rfl::Generic(std::string("x"));
  EXPECT_TRUE(obj1 != different_key);
}

TEST(generic, equality_of_nested_objects_and_arrays) {
  const auto make = []() {
    rfl::Object<rfl::Generic> inner;
    inner["value"] = rfl::Generic(int64_t{7});
    rfl::Generic::Array array{rfl::Generic(int64_t{1}),
                              rfl::Generic(std::move(inner))};
    rfl::Object<rfl::Generic> outer;
    outer["array"] = rfl::Generic(std::move(array));
    return rfl::Generic(std::move(outer));
  };
  EXPECT_TRUE(make() == make());
}

}  // namespace test_generic_equality
