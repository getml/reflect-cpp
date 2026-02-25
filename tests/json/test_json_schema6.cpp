#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"


namespace test_json_schema6 {

enum class Role {
  Normal = 0,
  Admin = 1,
  SuperAdmin = 0xFF,
};

struct User {
  std::string id;
  Role role;
  std::string username;
};

}  // namespace test_json_schema6


namespace rfl {
template <>
struct Reflector<test_json_schema6::Role> {
    using ReflType = uint32_t;

    static test_json_schema6::Role to(const ReflType& v) {
        test_json_schema6::Role rv = static_cast<test_json_schema6::Role>(v);
        switch (rv) {
            case test_json_schema6::Role::Normal:
            case test_json_schema6::Role::Admin:
            case test_json_schema6::Role::SuperAdmin:
                return rv;
            default:
                throw "invalid role value";
        }
    }

    static ReflType from(const test_json_schema6::Role& v) {
        return static_cast<ReflType>(v);
    }
};
};



namespace test_json_schema6 {

TEST(json, test_json_schema6) {
  const auto json_schema = rfl::json::to_schema<test_json_schema6::User>();

  const std::string expected =
      R"({"$schema":"https://json-schema.org/draft/2020-12/schema","$ref":"#/$defs/test_json_schema6__User","$defs":{"test_json_schema6__Role":{"type":"integer"},"test_json_schema6__User":{"type":"object","properties":{"id":{"type":"string"},"role":{"$ref":"#/$defs/test_json_schema6__Role"},"username":{"type":"string"}},"required":["id","role","username"]}}})";

  EXPECT_EQ(json_schema, expected);
}
}  // namespace test_json_schema6
