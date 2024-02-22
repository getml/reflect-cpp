#include "test_enum.hpp"

#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_enum {

enum class Color { red, green, blue, yellow };

struct Circle {
  float radius;
  Color color;
};

void test() {
  std::cout << std::source_location::current().function_name() << std::endl;

  const auto circle = Circle{.radius = 2.0, .color = Color::green};

  write_and_read(circle, R"({"radius":2.0,"color":"green"})");

  auto mutable_circle = circle;
  if (auto color = rfl::string_to_enum<Color>("red"); color) {
    mutable_circle.color = *color;
  }
  write_and_read(mutable_circle, R"({"radius":2.0,"color":"red"})");

  if (auto color = rfl::string_to_enum<Color>("bart"); color) {
    mutable_circle.color = *color;
  }
  write_and_read(mutable_circle, R"({"radius":2.0,"color":"red"})");

  if (auto color = rfl::string_to_enum<Color>(rfl::enum_to_string(Color::blue));
      color) {
    mutable_circle.color = *color;
  }
  write_and_read(mutable_circle, R"({"radius":2.0,"color":"blue"})");

  rfl::get_enumerators<Color>().apply([&](auto field) {
    if constexpr (decltype(field)::name() == "green") {
      mutable_circle.color = field.value();
    }
  });
  write_and_read(mutable_circle, R"({"radius":2.0,"color":"green"})");

  constexpr auto enumerator_array = rfl::get_enumerator_array<Color>();
  static_assert(enumerator_array[0].first == "red");
  static_assert(enumerator_array[1].first == "green");
  static_assert(enumerator_array[2].first == "blue");
  static_assert(enumerator_array[3].first == "yellow");
  static_assert(enumerator_array[0].second == Color::red);
  static_assert(enumerator_array[1].second == Color::green);
  static_assert(enumerator_array[2].second == Color::blue);
  static_assert(enumerator_array[3].second == Color::yellow);
}

}  // namespace test_enum
