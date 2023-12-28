#ifndef RFL_ALLOF_HPP_
#define RFL_ALLOF_HPP_

#include "Result.hpp"

namespace rfl {

/// Requires that all of the contraints C and Cs be true.
template <class C, class... Cs>
struct AllOf {
  template <class T>
  static rfl::Result<T> validate(T _value) noexcept {
    return validate_impl<T, C, Cs...>(_value);
  }

 private:
  template <class T, class Head, class... Tail>
  static rfl::Result<T> validate_impl(T _value) noexcept {
    if constexpr (sizeof...(Tail) == 0) {
      return Head::validate(_value);
    } else {
      return Head::validate(_value).and_then(validate_impl<T, Tail...>);
    }
  }
};

}  // namespace rfl

#endif
