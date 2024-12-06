#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace test_tagged_union5 {


  struct Sa {
    int a;
    std::string b;
  };

  struct Sb {
    std::string b;
    int i;
  };

  template <typename T, typename... Ts>
  concept OneOf = (std::is_same_v<T, Ts> || ...);

	template <typename T>
	bool operator==(const T& lhs, const T& rhs)
    requires OneOf<T,
                   struct test_tagged_union5::Sa,
                   struct test_tagged_union5::Sb>
	{
		return rfl::to_named_tuple(lhs) == rfl::to_named_tuple(rhs);
	}


  using Tu = rfl::TaggedUnion<"tu",Sa,Sb>;

  Tu v1 = Sa{.a = 1, .b = "b" };
  Tu v2 = Sa{.a = 1, .b = "b" };
  Tu v3 = Sa{.a = 1, .b = "bc" };
  Tu v4 = Sb{.b = "s", .i = -2};

  TEST(json, test_tagged_union5) {
    ASSERT_TRUE(v1 == v1);
    ASSERT_TRUE(v1 == v2);
    ASSERT_TRUE(v1 != v3);
    ASSERT_TRUE(v1 != v4);

    ASSERT_TRUE(v2 == v2);
    ASSERT_TRUE(v2 != v3);
    ASSERT_TRUE(v2 != v4);
    ASSERT_TRUE(v2 == v1);

    ASSERT_TRUE(v3 == v3);
    ASSERT_TRUE(v3 != v4);
    ASSERT_TRUE(v3 != v1);
    ASSERT_TRUE(v3 != v2);

    ASSERT_TRUE(v4 == v4);
    ASSERT_TRUE(v4 != v1);
    ASSERT_TRUE(v4 != v2);
    ASSERT_TRUE(v4 != v3);

    ASSERT_FALSE(v4 == v2);
    // This is a compile-time test
    EXPECT_TRUE(true);
  }

}  // namespace test_tagged_union5
