#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <cstdlib>
#include <cstddef>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

#include <rfl/env.hpp>
#include <rfl/json.hpp>

namespace test_env {

class ScopedEnvironment {
 public:
  ScopedEnvironment() : snapshot_(capture()) { clear(); }

  ~ScopedEnvironment() { restore(snapshot_); }

 private:
  using Environment = std::map<std::string, std::string>;

  static Environment capture() {
    auto env = Environment{};

    for (char** current = environ; *current != nullptr; ++current) {
      const std::string entry(*current);
      const auto pos = entry.find('=');

      if (pos == std::string::npos) {
        continue;
      }

      env.emplace(entry.substr(0, pos), entry.substr(pos + 1));
    }

    return env;
  }

  static void restore(const Environment& _snapshot) {
    const auto current = capture();

    for (const auto& [name, value] : current) {
      if (!_snapshot.contains(name)) {
        unsetenv(name.c_str());
      }
    }

    for (const auto& [name, value] : _snapshot) {
      setenv(name.c_str(), value.c_str(), 1);
    }
  }

  static void clear() {
    const auto current = capture();

    for (const auto& [name, value] : current) {
      unsetenv(name.c_str());
    }
  }

  Environment snapshot_;
};

template <class... Ps, class Assertions = std::nullptr_t>
void write_and_read(const auto& _obj, Assertions&& _assertions = nullptr) {
  using T = std::remove_cvref_t<decltype(_obj)>;

  const auto scoped_environment = ScopedEnvironment();

  rfl::env::write<Ps...>(_obj);

  if constexpr (!std::is_same_v<std::remove_cvref_t<Assertions>,
                                std::nullptr_t>) {
    _assertions();
  }

  const auto res = rfl::env::read<T, Ps...>();

  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();

  if (!res) {
    return;
  }

  const auto serialized1 = rfl::json::write<Ps...>(_obj);
  const auto serialized2 = rfl::json::write<Ps...>(res.value());

  EXPECT_EQ(serialized1, serialized2);
}

}  // namespace test_env

using test_env::write_and_read;

#endif
