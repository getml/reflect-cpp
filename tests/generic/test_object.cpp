#include <gtest/gtest.h>

#include <type_traits>

#include <rfl.hpp>

namespace test_object_difference_type_signed {

TEST(regression, object_difference_type_is_signed) {
  using DiffType = rfl::Object<int>::difference_type;
  EXPECT_TRUE(std::is_signed_v<DiffType>)
      << "Object::difference_type should be signed per C++ standard, "
         "but is unsigned (size_type)";
}

}  // namespace test_object_difference_type_signed
