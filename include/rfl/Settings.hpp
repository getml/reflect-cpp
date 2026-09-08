#ifndef RFL_SETTINGS_HPP_
#define RFL_SETTINGS_HPP_

#include <cstddef>
#include <type_traits>
#include <utility>

#include "Field.hpp"
#include "replace.hpp"

/// Defines the standard with<&T::field>(value) and with<"field">(value)
/// accessors inside a settings struct.
/// Fields should be declared const so the only way to
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
#define RFL_SETTINGS_OPS(Derived)                                              \
  template <auto _field_ptr>                                                   \
    requires(std::is_member_pointer_v<decltype(_field_ptr)>)                   \
  Derived with(auto _value) const {                                            \
    return ::rfl::replace(*this,                                               \
                          ::rfl::make_field<_field_ptr>(std::move(_value)));   \
  }                                                                            \
                                                                               \
  template <::rfl::internal::StringLiteral _name>                              \
  Derived with(auto _value) const {                                            \
    return ::rfl::replace(*this, ::rfl::make_field<_name>(std::move(_value))); \
  }

#endif
