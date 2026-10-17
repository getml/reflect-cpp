#include <string>

#include "write_and_read.hpp"
#include <test_rfl/rfl.hpp>
#include <test_rfl/rfl.json.hpp>


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
