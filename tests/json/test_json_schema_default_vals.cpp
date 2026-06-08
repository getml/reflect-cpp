#include <optional>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_schema_default {

struct Config {
  int port = 80;
  bool autostart = true;
};

struct DefaultValField {
  rfl::DefaultVal<int> with_default = 10;
};


struct DefaultWithConfig {
  rfl::DefaultVal<Config> with_default = Config{443, true};
};

TEST(json, test_with_default) {
   auto json_schema = rfl::json::to_schema<DefaultValField>();

   std::string expected =      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_schema_default__DefaultValField","$defs":{"test_schema_default__DefaultValField":{"type":"object","properties":{"with_default":{"type":"integer","default":"10"}},"required":[]}}})";

  EXPECT_EQ(json_schema, expected) << json_schema ;

   json_schema = rfl::json::to_schema<DefaultWithConfig>();
   expected =      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_schema_default__DefaultWithConfig","$defs":{"test_schema_default__Config":{"type":"object","properties":{"port":{"type":"integer"},"autostart":{"type":"boolean"}},"required":["port","autostart"]},"test_schema_default__DefaultWithConfig":{"type":"object","properties":{"with_default":{"$ref":"#/$defs/test_schema_default__Config","default":"{\"port\":443,\"autostart\":true}"}},"required":[]}}})";

   EXPECT_EQ(json_schema, expected) << "is " << json_schema ;

}



}  // namespace test_schema_default
