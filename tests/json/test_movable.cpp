#include <cassert>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>

#include "write_and_read.hpp"


struct MoveableType {
    int x;


    MoveableType(MoveableType&&) = default;
    MoveableType(const MoveableType&) = delete;

    using ReflectionType = int;
    MoveableType(int&& x) : x(x) {}
    int reflection() const { return x; }
};

TEST(json, test_moveable) {
    MoveableType moveable = {2};

    write_and_read(
        moveable, R"(2)");
}