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

  EXPECT_EQ(xml_string,
            R"(<?xml version="1.0" encoding="UTF-8"?>
<cat>
    <name>Cachou</name>
    <colour>black</colour>
    <species>cat</species>
</cat>
)");
}
}  // namespace test_generic
