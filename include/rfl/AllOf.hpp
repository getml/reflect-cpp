#ifndef RFL_ALLOF_HPP_
#define RFL_ALLOF_HPP_

#include <vector>

#include "Result.hpp"
#include "parsing/schema/ValidationType.hpp"

namespace rfl {

/// Requires that all of the contraints C and Cs be true.
template <class C, class... Cs>
struct AllOf {
  template <class T>
  static rfl::Result<T> validate(T _value) noexcept {
    return validate_impl<T, C, Cs...>(_value);
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto types = std::vector<ValidationType>(
        {C::template to_schema<T>(), Cs::template to_schema<T>()...});
    return ValidationType{ValidationType::AllOf{.types_ = types}};
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
