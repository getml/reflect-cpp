#include <rfl.hpp>
#include <rfl/json.hpp>

#include "../check_fail_fast.hpp"

TEST(json, test_fail_fast_round_trip) {
  test_fail_fast::check_round_trip(
      [](const auto& input) { return rfl::json::write(input); },
      []<class T>(const auto& bytes) {
        return rfl::json::read<T, rfl::FailFast>(bytes);
      });
}

namespace {
struct TwoFields {
  int first;
  int second;
};

struct Counted {
  using ReflectionType = int;
  static inline int live = 0;
  static inline int constructed = 0;
  int value;
  explicit Counted(int v) : value(v) {
    ++live;
    ++constructed;
  }
  Counted(Counted&& other) noexcept : value(other.value) { ++live; }
  ~Counted() { --live; }
  int reflection() const { return value; }
};

struct OwningRecord {
  Counted first;
  int invalid;
  Counted skipped;
};
}  // namespace

TEST(json, test_fail_fast_first_error) {
  const auto result = rfl::json::read<TwoFields, rfl::FailFast>(
      R"({"first":"bad","second":"also bad"})");
  ASSERT_FALSE(result);
  EXPECT_EQ(result.error().what(),
            "Failed to parse field 'first': Could not cast to int.");

  const auto missing = rfl::json::read<TwoFields, rfl::FailFast>("{}");
  ASSERT_FALSE(missing);
  EXPECT_EQ(missing.error().what(), "Field named 'first' not found.");
}

TEST(json, test_fail_fast_partial_object_cleanup) {
  Counted::constructed = 0;
  const auto result = rfl::json::read<OwningRecord, rfl::FailFast>(
      R"({"first":1,"invalid":"bad","skipped":2})");
  ASSERT_FALSE(result);
  EXPECT_EQ(Counted::constructed, 1);
  EXPECT_EQ(Counted::live, 0);
}

TEST(json, test_fail_fast_processors) {
  using Ps = rfl::Processors<rfl::FailFast, rfl::NoFieldNames>;
  const auto result = rfl::json::read<TwoFields, Ps>(R"(["bad","bad"])");
  ASSERT_FALSE(result);
  EXPECT_EQ(result.error().what(),
            "Failed to parse field 'first': Could not cast to int.");

  const auto defaults =
      rfl::json::read<TwoFields, rfl::FailFast, rfl::DefaultIfMissing>(
          R"({"first":7})");
  ASSERT_TRUE(defaults);
  EXPECT_EQ(defaults->first, 7);
  EXPECT_EQ(defaults->second, 0);

  const auto extra =
      rfl::json::read<TwoFields, rfl::FailFast, rfl::NoExtraFields>(
          R"({"extra":0,"first":"bad","second":"bad"})");
  ASSERT_FALSE(extra);
  EXPECT_EQ(extra.error().what(), "Value named 'extra' not used.");
}

TEST(json, test_fail_fast_defaults_and_stripped_names) {
  const auto bad =
      rfl::json::read<TwoFields, rfl::FailFast, rfl::DefaultIfMissing,
                      rfl::NoFieldNames>(R"(["bad","also bad"])");
  ASSERT_FALSE(bad);
  EXPECT_EQ(bad.error().what(),
            "Failed to parse field 'first': Could not cast to int.");
  const auto good =
      rfl::json::read<TwoFields, rfl::FailFast, rfl::DefaultIfMissing,
                      rfl::NoFieldNames>("[7]");
  ASSERT_TRUE(good);
  EXPECT_EQ(good->first, 7);
  EXPECT_EQ(good->second, 0);
}

TEST(json, test_fail_fast_nested_error) {
  struct Nested {
    TwoFields object;
    int other;
  };
  const auto result = rfl::json::read<Nested, rfl::FailFast>(
      R"({"object":{"first":"bad","second":"bad"},"other":"bad"})");
  ASSERT_FALSE(result);
  EXPECT_EQ(result.error().what(),
            "Failed to parse field 'object': Failed to parse field 'first': "
            "Could not cast to int.");
}

TEST(json, test_fail_fast_error_policy) {
  test_fail_fast::check_error_policy(
      [](const auto& input) { return rfl::json::write(input); },
      []<class T>(const auto& bytes) { return rfl::json::read<T>(bytes); },
      []<class T>(const auto& bytes) {
        return rfl::json::read<T, rfl::FailFast>(bytes);
      });
}

TEST(json, test_fail_fast_default_value_missing_fields) {
  struct WithDefault {
    int first;
    int second;
    rfl::DefaultVal<std::string> label = "default";
  };
  const auto result = rfl::json::read<WithDefault, rfl::FailFast>("{}");
  ASSERT_FALSE(result);
  EXPECT_EQ(result.error().what(), "Field named 'first' not found.");

  const auto valid =
      rfl::json::read<WithDefault, rfl::FailFast>(R"({"first":1,"second":2})");
  ASSERT_TRUE(valid);
  EXPECT_EQ(valid->label.value(), "default");
}

TEST(json, test_fail_fast_named_tuple) {
  using Named =
      rfl::NamedTuple<rfl::Field<"first", int>, rfl::Field<"second", int>>;
  const auto result = rfl::json::read<Named, rfl::FailFast>(
      R"({"first":"bad","second":"also bad"})");
  ASSERT_FALSE(result);
  EXPECT_EQ(result.error().what(),
            "Failed to parse field 'first': Could not cast to int.");
}
