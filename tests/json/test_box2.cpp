#include <gtest/gtest.h>

#include <cassert>
#include <string>
#include <memory>
#include <utility>

import rfl;
import rfl.json;


namespace test_box2 {

TEST(json, test_box2) {
  auto ptr = std::make_unique<std::string>("Hello World!");
  const rfl::Result<rfl::Box<std::string>> box =
      rfl::make_box<std::string>(std::move(ptr));

  ASSERT_TRUE(box && true);
}
}  // namespace test_box2
