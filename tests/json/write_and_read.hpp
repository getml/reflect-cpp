#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl/json.hpp>
#include <string>
#include <type_traits>

template <class... Ps>
auto write(const auto& _struct, const std::string& _expected) {
  const auto json_string1 = rfl::json::write<Ps...>(_struct);
  EXPECT_EQ(json_string1, _expected)
      << "Test failed on write. Expected:" << std::endl
      << _expected << std::endl
      << "Got: " << std::endl
      << json_string1 << std::endl
      << std::endl;
  return json_string1;
}

template <class... Ps>
void read(const std::string& _json, const auto& _expected) {
  using T = std::remove_cvref_t<decltype(_expected)>;
  const auto res = rfl::json::read<T, Ps...>(_json);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().value().what();
}

template <class... Ps>
void write_and_read(const auto& _struct, const std::string& _expected) {
  using T = std::remove_cvref_t<decltype(_struct)>;
  const auto json_string1 = rfl::json::write<Ps...>(_struct);
  EXPECT_EQ(json_string1, _expected)
      << "Test failed on write. Expected:" << std::endl
      << _expected << std::endl
      << "Got: " << std::endl
      << json_string1 << std::endl
      << std::endl;
  const auto res = rfl::json::read<T, Ps...>(json_string1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().value().what();
  const auto json_string2 = rfl::json::write<Ps...>(res.value());
  EXPECT_EQ(json_string2, _expected)
      << "Test failed on read. Expected:" << std::endl
      << _expected << std::endl
      << "Got: " << std::endl
      << json_string2 << std::endl
      << std::endl;
}

#endif
