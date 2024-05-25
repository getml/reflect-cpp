#include <iostream>
#include <source_location>
#include <string>

#include "rfl.hpp"
#include "rfl/json.hpp"
#include "write_and_read.hpp"

namespace test_c_array_class1 {

struct Test1 {
  std::vector<std::string> classes[3];
};

TEST(json, test_c_array_class1) {
  Test1 test1 = {.classes = {{"Little A", "Little B", "Little C"},
                             {"BIG A", "BIG B", "BIG C"},
                             {"??", "$%", "#@"}}};
  write_and_read(
      test1,
      R"({"classes":[["Little A","Little B","Little C"],["BIG A","BIG B","BIG C"],["??","$%","#@"]]})");
}

}  // namespace test_c_array_class1
