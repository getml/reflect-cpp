#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "write_and_read.hpp"

namespace test_apply_on_named_tuple {

TEST(json, test_apply_on_named_tuple) {
  auto person = rfl::Field<"first_name", std::string>("Bart") *
                rfl::Field<"last_name", std::string>("Simpson");

  const auto print = [](const auto& f) {
    const auto& name = f.name();
    const auto& value = f.value();
    std::cout << name << ", " << value << std::endl;
  };

  person.apply(print);

  EXPECT_EQ(rfl::json::write(person),
            R"({"first_name":"Bart","last_name":"Simpson"})");
}
}  // namespace test_apply_on_named_tuple
