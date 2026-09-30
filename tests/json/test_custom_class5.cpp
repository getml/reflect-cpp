#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <type_traits>
#include <vector>

#include "write_and_read.hpp"

namespace test_custom_class5 {

/// Uses a static `from_reflection` factory instead of a converting
/// constructor. Because no constructor is declared, `Config` remains an
/// aggregate and can still be built with a designated initializer list.
struct Config {
  struct ConfigImpl {
    rfl::Rename<"portNumber", int> port;
    std::string host = "localhost";
    std::vector<std::string> tags;
  };

  using ReflectionType = ConfigImpl;

  static Config from_reflection(ReflectionType&& _impl) {
    return Config{.port = _impl.port(),
                  .host = _impl.host,
                  .tags = _impl.tags};
  }

  ReflectionType reflection() const {
    return ReflectionType{.port = port, .host = host, .tags = tags};
  }

  int port;
  std::string host;
  std::vector<std::string> tags;
};

// The whole point of the factory: no constructor is declared, so designated
// initializers keep working.
static_assert(std::is_aggregate_v<Config>);

TEST(json, test_custom_class5) {
  const auto config =
      Config{.port = 8080, .host = "example.com", .tags = {"a", "b"}};

  write_and_read(
      config,
      R"({"portNumber":8080,"host":"example.com","tags":["a","b"]})");
}

}  // namespace test_custom_class5
