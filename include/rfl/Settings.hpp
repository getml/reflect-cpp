#ifndef RFL_SETTINGS_HPP_
#define RFL_SETTINGS_HPP_

#include <cstddef>
#include <type_traits>
#include <utility>

#include "internal/cpp20/settings.hpp"

/// Defines the standard with<&T::field>(value) and with<"field">(value)
/// accessors inside a settings struct. The struct must be a flat aggregate
/// (no base classes). Fields should be declared const so the only way to
/// mutate them is via with(), which returns a new copy with the chosen field
/// replaced. Place the macro at the end of the struct body, after all data
/// members.
///
/// Usage:
///   struct MySettings {
///     const int some_option = 42;
///     RFL_SETTINGS_OPS(MySettings)
///   };
///
///   auto a = MySettings{}.with<&MySettings::some_option>(100);
///   auto b = MySettings{}.with<"some_option">(100);
#define RFL_SETTINGS_OPS(Derived)                                             \
  template <auto FieldPtr>                                                    \
    requires ::rfl::internal::cpp20::const_member_of<FieldPtr, Derived>       \
  Derived                                                                     \
  with(std::remove_const_t<                                                   \
       std::remove_reference_t<decltype(std::declval<Derived>().*FieldPtr)>>  \
           _value) const {                                                    \
    return ::rfl::internal::cpp20::settings_with_replaced<Derived, FieldPtr>( \
        *this, std::move(_value));                                            \
  }                                                                           \
  template <::rfl::internal::StringLiteral Name>                              \
  Derived                                                                     \
  with(::rfl::internal::cpp20::field_value_type_at_t<                         \
       Derived, ::rfl::internal::cpp20::field_index_by_name_v<Derived, Name>> \
           _value) const {                                                    \
    return ::rfl::internal::cpp20::settings_with_replaced_by_name<Derived,    \
                                                                  Name>(      \
        *this, std::move(_value));                                            \
  }

#endif
