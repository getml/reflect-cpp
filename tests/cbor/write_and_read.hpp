#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl/cbor.hpp>
#include <string>

template <class T>
void write_and_read(const T& _struct) {
  const auto serialized1 = rfl::cbor::write(_struct);
  const auto res = rfl::cbor::read<T>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().value().what();
  const auto serialized2 = rfl::cbor::write(res.value());
  EXPECT_EQ(serialized1, serialized2);
}

#endif
