#include <gtest/gtest.h>

#include <rfl.hpp>
#include <rfl/env.hpp>

namespace test_literal_map {

using FieldName = rfl::Literal<"firstName", "lastName">;

TEST(env, test_literal_map) {
  std::map<FieldName, std::unique_ptr<std::string>> homer;
  homer.insert(std::make_pair(FieldName::make<"firstName">(),
                              std::make_unique<std::string>("Homer")));
  homer.insert(std::make_pair(FieldName::make<"lastName">(),
                              std::make_unique<std::string>("Simpson")));
}
}  // namespace test_literal_map
