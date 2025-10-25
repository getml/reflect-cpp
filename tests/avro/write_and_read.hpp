#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <rfl/avro.hpp>
#include <rfl/json.hpp>
#include <string>

template <class... Ps>
void write_and_read(const auto& _struct) {
  using T = std::remove_cvref_t<decltype(_struct)>;
  const auto serialized1 = rfl::avro::write<Ps...>(_struct);
  const auto res = rfl::avro::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::avro::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2);
}

template <class... Ps>
void write_and_read(const auto& _struct, const std::string& _json_str) {
  using T = std::remove_cvref_t<decltype(_struct)>;
  const auto serialized1 = rfl::avro::write<Ps...>(_struct);
  const auto res = rfl::avro::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::avro::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2);
  std::cout << rfl::json::write(res.value());
}
#endif
