#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/xml.hpp>
#include <string>

template <
    rfl::internal::StringLiteral _root_name = rfl::internal::StringLiteral(""),
    class T>
void write_and_read(const T& _struct) {
  const auto serialized1 = rfl::xml::write<_root_name>(_struct);
  const auto res = rfl::xml::read<T>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().value().what();
  const auto serialized2 = rfl::xml::write<_root_name>(res.value());
  EXPECT_EQ(serialized1, serialized2);
}

#endif
