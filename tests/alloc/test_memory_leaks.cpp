#include <gtest/gtest.h>

#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <rfl.hpp>
#include <rfl/json.hpp>

#include "alloc_tracking.hpp"

// Parser_string_view allocates memory with new[] but std::string_view
// does not own it — guaranteed memory leak.
// File: include/rfl/parsing/Parser_string_view.hpp:40-43
namespace test_parser_string_view_leak {

struct StringViewHolder {
  std::string_view text;
};

TEST(alloc, parser_string_view_does_not_leak) {
  alloc_tracking::Guard guard;

  {
    const auto json = R"({"text":"hello"})";
    const auto result =
        rfl::json::read<StringViewHolder, rfl::AllowRawPtrs>(json);
    ASSERT_TRUE(result) << result.error().what();
    EXPECT_EQ(result.value().text, "hello");
    // Result goes out of scope here. If the implementation properly manages
    // memory, all new[] allocations should have matching delete[].
  }

  EXPECT_EQ(guard.leaks(), 0u)
      << "Parser_string_view leaks memory: allocates with new char[] "
         "(Parser_string_view.hpp:41) but std::string_view does not own "
         "the memory. " << guard.leaks() << " allocation(s) not freed.";
}

}  // namespace test_parser_string_view_leak

// Parser_span allocates memory with new[] but std::span does not own it
// — guaranteed memory leak.
// File: include/rfl/parsing/Parser_span.hpp:45-52
namespace test_parser_span_leak {

struct SpanHolder {
  std::span<int> values;
};

TEST(alloc, parser_span_does_not_leak) {
  alloc_tracking::Guard guard;

  {
    const auto json = R"({"values":[1,2,3]})";
    const auto result =
        rfl::json::read<SpanHolder, rfl::AllowRawPtrs>(json);
    ASSERT_TRUE(result) << result.error().what();

    const auto& sp = result.value().values;
    ASSERT_EQ(sp.size(), 3u);
    EXPECT_EQ(sp[0], 1);
    EXPECT_EQ(sp[1], 2);
    EXPECT_EQ(sp[2], 3);
  }

  EXPECT_EQ(guard.leaks(), 0u)
      << "Parser_span leaks memory: allocates with new Type[] "
         "(Parser_span.hpp:45) but std::span does not own the memory. "
      << guard.leaks() << " allocation(s) not freed.";
}

}  // namespace test_parser_span_leak

// Multiple deserializations should not accumulate leaks.
// This test amplifies the leak to make it more visible.
namespace test_repeated_leak {

struct StringViewHolder {
  std::string_view text;
};

TEST(alloc, repeated_string_view_deserialization_does_not_accumulate_leaks) {
  alloc_tracking::Guard guard;

  {
    for (int i = 0; i < 10; ++i) {
      const auto json = R"({"text":"iteration"})";
      auto result =
          rfl::json::read<StringViewHolder, rfl::AllowRawPtrs>(json);
      (void)result;
    }
  }

  EXPECT_EQ(guard.leaks(), 0u)
      << "10 deserializations into string_view produced " << guard.leaks()
      << " leaked allocation(s). Each deserialization leaks once.";
}

}  // namespace test_repeated_leak

// Verify that normal std::string deserialization does NOT leak.
// This is a sanity check that the tracking infrastructure works correctly.
namespace test_string_no_leak {

struct StringHolder {
  std::string text;
};

TEST(alloc, string_deserialization_does_not_leak) {
  alloc_tracking::Guard guard;

  {
    const auto json = R"({"text":"hello"})";
    const auto result = rfl::json::read<StringHolder>(json);
    ASSERT_TRUE(result) << result.error().what();
    EXPECT_EQ(result.value().text, "hello");
  }

  EXPECT_EQ(guard.leaks(), 0u)
      << "std::string deserialization should not leak, but "
      << guard.leaks() << " allocation(s) were not freed. "
         "This indicates a problem with the tracking infrastructure.";
}

}  // namespace test_string_no_leak

// Verify that normal std::vector deserialization does NOT leak.
namespace test_vector_no_leak {

struct VectorHolder {
  std::vector<int> values;
};

TEST(alloc, vector_deserialization_does_not_leak) {
  alloc_tracking::Guard guard;

  {
    const auto json = R"({"values":[1,2,3]})";
    const auto result = rfl::json::read<VectorHolder>(json);
    ASSERT_TRUE(result) << result.error().what();
    EXPECT_EQ(result.value().values.size(), 3u);
  }

  EXPECT_EQ(guard.leaks(), 0u)
      << "std::vector deserialization should not leak, but "
      << guard.leaks() << " allocation(s) were not freed. "
         "This indicates a problem with the tracking infrastructure.";
}

}  // namespace test_vector_no_leak
