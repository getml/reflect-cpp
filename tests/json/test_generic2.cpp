#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_generic2 {

TEST(json, test_generic2) {
  std::string response =
      "{\"kind\":2,\"error\":\"Internal "
      "error\",\"status\":101,\"stats\":{\"cpuTime\":803,\"totalTime\":1}}";

  struct DataHolder {
    int kind = 0;
    rfl::Generic data;
  };
  auto result = rfl::json::read<DataHolder>(response);

  EXPECT_TRUE(!(result && true));
}

}  // namespace test_generic2
