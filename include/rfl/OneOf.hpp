#ifndef RFL_ONEOF_HPP_
#define RFL_ONEOF_HPP_

#include <string>
#include <utility>
#include <vector>

#include "Result.hpp"
#include "parsing/schema/ValidationType.hpp"

namespace rfl {

/// Requires that all of the contraints C and Cs be true.
template <class C, class... Cs>
struct OneOf {
  template <class T>
  static rfl::Result<T> validate(const T& _value) noexcept {
    return validate_impl<T, C, Cs...>(_value, {});
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto types = std::vector<ValidationType>(
        {C::template to_schema<T>(), Cs::template to_schema<T>()...});
    return ValidationType{ValidationType::OneOf{.types_ = types}};
  }

 private:
  static Error make_error_message(const std::vector<Error>& _errors) {
    std::string msg = "Expected exactly 1 out of " +
                      std::to_string(sizeof...(Cs) + 1) +
                      " validations to pass, but " +
                      std::to_string(sizeof...(Cs) + 1 - _errors.size()) +
                      " of them did. The following errors were generated: ";
    for (size_t i = 0; i < _errors.size(); ++i) {
      msg += "\n" + std::to_string(i + 1) + ") " + _errors.at(i).what();
    }
    return Error(msg);
  }

  template <class T, class Head, class... Tail>
  static rfl::Result<T> validate_impl(const T& _value,
                                      std::vector<Error> _errors) {
    const auto push_back = [&](Error&& _err) -> rfl::Result<T> {
      _errors.emplace_back(std::forward<Error>(_err));
      return _err;
    };

    const auto next_validation = [&](rfl::Result<T>&& _r) -> rfl::Result<T> {
      _r.or_else(push_back);

      if constexpr (sizeof...(Tail) == 0) {
        if (_errors.size() == sizeof...(Cs)) {
          return _value;
        }
        return make_error_message(_errors);
      } else {
        return validate_impl<T, Tail...>(
            _value, std::forward<std::vector<Error>>(_errors));
      }
    };

    return Head::validate(_value)
        .and_then(next_validation)
        .or_else(next_validation);
  }
};

}  // namespace rfl

#endif
