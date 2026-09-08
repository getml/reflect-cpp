#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <rfl/boost_serialization.hpp>
#include <rfl/json.hpp>
#include <string>

template <class... Ps>
void write_and_read(const auto& _struct) {
  using T = std::remove_cvref_t<decltype(_struct)>;
  const auto serialized1 = rfl::boost_serialization::write<Ps...>(_struct);
  const auto res = rfl::boost_serialization::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::boost_serialization::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2);
}

template <class... Ps>
void write_and_read_with_json(const auto& _struct) {
  using T = std::remove_cvref_t<decltype(_struct)>;
  const auto serialized1 = rfl::boost_serialization::write<Ps...>(_struct);
  const auto res = rfl::boost_serialization::read<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().what();
  const auto serialized2 = rfl::boost_serialization::write<Ps...>(res.value());
  EXPECT_EQ(serialized1, serialized2);
  EXPECT_EQ(rfl::json::write<Ps...>(_struct),
            rfl::json::write<Ps...>(res.value()));
}
#endif
