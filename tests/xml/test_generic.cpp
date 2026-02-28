#include <cassert>
#include <rfl.hpp>
#include <string>

#include "write_and_read.hpp"

namespace test_generic {

TEST(xml, test_generic) {
  rfl::Generic::Object cat;
  cat["name"] = "Cachou";
  cat["colour"] = "black";
  cat["species"] = "cat";

  const auto xml_string = rfl::xml::write<"cat">(cat);

  std::cout << xml_string << std::endl;

  const auto generic_cat =
      rfl::xml::read<rfl::Object<rfl::Generic>>(xml_string);

  std::cout << rfl::xml::write<"cat">(generic_cat) << std::endl;
}
}  // namespace test_generic
