#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdlib>
#include <map>
#include <rfl/env.hpp>
#include <rfl/json.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace test_env {

inline void portable_setenv(const std::string& name, const std::string& value) {
#ifdef _WIN32
  _putenv_s(name.c_str(), value.c_str());
#else
  setenv(name.c_str(), value.c_str(), 1);
#endif
}

inline void portable_unsetenv(const std::string& name) {
#ifdef _WIN32
  _putenv_s(name.c_str(), "");
#else
  unsetenv(name.c_str());
#endif
}

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
        portable_unsetenv(name.c_str());
      }
    }

    for (const auto& [name, value] : _snapshot) {
      portable_setenv(name.c_str(), value.c_str());
    }
  }

  static void clear() {
    const auto current = capture();

    for (const auto& [name, value] : current) {
      portable_unsetenv(name.c_str());
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
