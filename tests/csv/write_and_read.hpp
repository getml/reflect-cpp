#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <rfl/csv.hpp>

template <class... Ps>
void write_and_read(const auto& _vec, const rfl::csv::Settings& _settings =
                                          rfl::csv::Settings{}) {
  using T = std::remove_cvref_t<decltype(_vec)>;
  const auto serialized1 = rfl::csv::write<Ps...>(_vec, _settings);
  const auto res = rfl::csv::read<T, Ps...>(serialized1, _settings);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::csv::write<Ps...>(res.value(), _settings);
  EXPECT_EQ(serialized1, serialized2);
}

#endif
