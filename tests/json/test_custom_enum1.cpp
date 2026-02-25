#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_custom_enum1 {

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

}  // namespace test_custom_enum1


namespace rfl {
template <>
struct Reflector<test_custom_enum1::Role> {
    using ReflType = uint32_t;

    static test_custom_enum1::Role to(const ReflType& v) {
        test_custom_enum1::Role rv = static_cast<test_custom_enum1::Role>(v);
        switch (rv) {
            case test_custom_enum1::Role::Normal:
            case test_custom_enum1::Role::Admin:
            case test_custom_enum1::Role::SuperAdmin:
                return rv;
            default:
                throw "invalid role value";
        }
    }

    static ReflType from(const test_custom_enum1::Role& v) {
        return static_cast<ReflType>(v);
    }
};
};

namespace test_custom_enum1 {

TEST(json, test_custom_enum1) {
  const auto test1 = test_custom_enum1::User{
    .id = "123",
    .role = test_custom_enum1::Role::SuperAdmin,
    .username = "bart",
  };
  const auto test2 = rfl::json::read<test_custom_enum1::User>(
    rfl::json::write(test1)).value();

  write_and_read(test2, R"({"id":"123","role":255,"username":"bart"})");
}
}  // namespace test_custom_class4
