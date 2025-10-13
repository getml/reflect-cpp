#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <rfl/parquet.hpp>

template <class... Ps>
void write_and_read(const auto& _vec, const rfl::parquet::Settings& _settings =
                                          rfl::parquet::Settings{}) {
  using T = std::remove_cvref_t<decltype(_vec)>;
  const auto serialized1 = rfl::parquet::write<Ps...>(_vec, _settings);
  const auto res = rfl::parquet::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::parquet::write<Ps...>(res.value(), _settings);
  EXPECT_EQ(serialized1, serialized2);
}

#endif
