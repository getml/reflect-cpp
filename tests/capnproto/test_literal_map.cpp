#include <iostream>
#include <map>
#include <memory>
#include <rfl.hpp>
#include <string>
#include <unordered_map>

#include "write_and_read.hpp"

namespace test_literal_map {

using FieldName = rfl::Literal<"firstName", "lastName">;

struct RootStruct {
  std::map<FieldName, std::unique_ptr<std::string>> root;
};

TEST(capnproto, test_literal_map) {
  RootStruct homer;
  homer.root.insert(std::make_pair(FieldName::make<"firstName">(),
                                   std::make_unique<std::string>("Homer")));
  homer.root.insert(std::make_pair(FieldName::make<"lastName">(),
                                   std::make_unique<std::string>("Simpson")));
  write_and_read(homer);
}
}  // namespace test_literal_map
