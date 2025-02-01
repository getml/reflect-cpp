#ifndef RFL_ONEOF_HPP_
#define RFL_ONEOF_HPP_

#include <sstream>
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
  static std::string make_error_message(const std::vector<Error>& _errors) {
    std::stringstream stream;
    stream << "Expected exactly 1 out of " << sizeof...(Cs) + 1
           << " validations to pass, but " << sizeof...(Cs) + 1 - _errors.size()
           << " of them did. The following errors were generated: ";
    for (size_t i = 0; i < _errors.size(); ++i) {
      stream << "\n" << i + 1 << ") " << _errors.at(i).what();
    }
    return stream.str();
  }

  template <class T, class Head, class... Tail>
  static rfl::Result<T> validate_impl(const T& _value,
                                      std::vector<Error> _errors) {
    return Head::validate(_value)
        .and_then([&](auto&& _res) -> rfl::Result<T> {
          if constexpr (sizeof...(Tail) == 0) {
            if (_errors.size() == sizeof...(Cs)) {
              return _value;
            }
            return error(make_error_message(_errors));
          } else {
            return validate_impl<T, Tail...>(_value, std::move(_errors));
          }
        })
        .or_else([&](auto&& _err) -> rfl::Result<T> {
          _errors.emplace_back(std::move(_err));
          if constexpr (sizeof...(Tail) == 0) {
            return error(make_error_message(_errors));
          } else {
            return validate_impl<T, Tail...>(_value, std::move(_errors));
          }
        });
  }
};

}  // namespace rfl

#endif
