#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <gtest/gtest.h>

#include <iostream>
#include <rfl/json.hpp>
#include <string>

template <class... Ps>
void write_and_read(const auto& _struct) {
  using T = std::remove_cvref_t<decltype(_struct)>;
  const auto serialized1 = rfl::to_generic<Ps...>(_struct);
  EXPECT_EQ(rfl::json::write(serialized1), rfl::json::write<Ps...>(_struct));
  const auto res = rfl::from_generic<T, Ps...>(serialized1);
  EXPECT_TRUE(res && true) << "Test failed on read. Error: "
                           << res.error().value().what();
  EXPECT_EQ(rfl::json::write(serialized1),
            rfl::json::write<Ps...>(res.value()));
}

#endif
