#include <cassert>
#include <concepts>
#include <rfl.hpp>

#include "write_and_read.hpp"

namespace test_tagged_union {

struct Circle {
  double radius;
};

struct Rectangle {
  double height;
  double width;
};

struct Square {
  double width;
};

using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

TEST(generic, test_tagged_union) { 
  const Shapes r = Rectangle{.height = 10, .width = 5};
  write_and_read(r);
}
}  // namespace test_tagged_union

namespace test_tagged_union_discriminator {

struct A {
  int x;
};

struct B {
  std::string y;
};

using TU = rfl::TaggedUnion<"type", A, B>;

template <class T>
concept has_discriminator = requires { T::discriminator_; };

template <class T>
concept has_discrimininator = requires { T::discrimininator_; };

TEST(regression, tagged_union_discriminator_spelling) {
  constexpr bool correct_name = has_discriminator<TU>;
  constexpr bool typo_name = has_discrimininator<TU>;
  EXPECT_TRUE(correct_name)
      << "TaggedUnion should have member 'discriminator_', "
         "not 'discrimininator_' (typo with extra 'n')";
  EXPECT_FALSE(typo_name)
      << "TaggedUnion has 'discrimininator_' (typo) which should be "
         "'discriminator_'";
}

}  // namespace test_tagged_union_discriminator
