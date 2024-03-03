#ifndef RFL_PARSING_SCHEMA_TONUMERICTYPE_HPP_
#define RFL_PARSING_SCHEMA_TONUMERICTYPE_HPP_

#include <type_traits>

#include "../../always_false.hpp"
#include "Type.hpp"

namespace rfl::parsing::schema {

template <class T>
inline schema::Type::NumericType to_numeric_type() {
  using U = std::remove_cvref_t<T>;

  if constexpr (std::is_same<U, std::int32_t>()) {
    return Type::NumericType(Type::Int32{});

  } else if constexpr (std::is_same<U, std::int64_t>()) {
    return Type::NumericType(Type::Int64{});

  } else if constexpr (std::is_same<U, std::uint32_t>()) {
    return Type::NumericType(Type::UInt32{});

  } else if constexpr (std::is_same<U, std::uint64_t>()) {
    return Type::NumericType(Type::UInt64{});

  } else if constexpr (std::is_integral<U>()) {
    return Type::NumericType(Type::Integer{});

  } else if constexpr (std::is_same<U, float>()) {
    return Type::NumericType(Type::Float{});

  } else if constexpr (std::is_floating_point_v<U>) {
    return Type::NumericType(Type::Double{});

  } else {
    static_assert(always_false_v<U>, "Not a numeric value.");
  }
}

}  // namespace rfl::parsing::schema

#endif
