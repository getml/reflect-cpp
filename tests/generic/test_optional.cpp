#include <string>
#include <gtest/gtest.h>
#include <optional>

#include <test_rfl/rfl.hpp>


namespace test_optional {

struct Person {
    std::string name;
    std::optional<rfl::Box<Person>> spouse;
};

TEST(generic, test_optional) {
    rfl::Generic generic;
    // This failed to compile due to optional copying rather than moving
    rfl::from_generic<Person>(generic);
}
}  // namespace test_map
