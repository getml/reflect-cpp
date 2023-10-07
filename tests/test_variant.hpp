#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

// Makes sure that we can set default values.
void test_variant() {
    std::cout << "test_variant" << std::endl;

    struct Circle {
        rfl::Field<"radius", double> radius;
    };

    struct Rectangle {
        rfl::Field<"height", double> height;
        rfl::Field<"width", double> width;
    };

    struct Square {
        rfl::Field<"width", double> width;
    };

    using Shapes = std::variant<Circle, Rectangle, std::unique_ptr<Square>>;

    const Shapes r = Rectangle{.height = 10, .width = 5};

    write_and_read(r, "{\"height\":10.0,\"width\":5.0}");
}
