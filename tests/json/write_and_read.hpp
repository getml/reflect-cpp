#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl/json.hpp>
#include <string>

template <class T>
void write_and_read(const T& _struct, const std::string& _expected) {
  const auto json_string1 = rfl::json::write(_struct);
  EXPECT_EQ(json_string1, _expected)
      << "Test failed on write. Expected:" << std::endl
      << _expected << std::endl
      << "Got: " << std::endl
      << json_string1 << std::endl
      << std::endl;
  const auto res = rfl::json::read<T>(json_string1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().value().what();
  const auto json_string2 = rfl::json::write(res.value());
  EXPECT_EQ(json_string2, _expected)
      << "Test failed on read. Expected:" << std::endl
      << _expected << std::endl
      << "Got: " << std::endl
      << json_string2 << std::endl
      << std::endl;
}

#endif
