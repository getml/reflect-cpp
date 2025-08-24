#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl/parquet.hpp>
#include <string>

template <class... Ps>
void write_and_read(const auto& _vec) {
  using T = std::remove_cvref_t<decltype(_vec)>;
  const auto serialized1 = rfl::parquet::write<Ps...>(_vec);
  const auto res = rfl::parquet::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::parquet::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2);
}

#endif
