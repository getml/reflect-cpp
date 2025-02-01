#ifndef RFL_ANYOF_HPP_
#define RFL_ANYOF_HPP_

#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Result.hpp"
#include "parsing/schema/ValidationType.hpp"

namespace rfl {

/// Requires that all of the contraints C and Cs be true.
template <class C, class... Cs>
struct AnyOf {
  template <class T>
  static rfl::Result<T> validate(const T& _value) noexcept {
    return validate_impl<T, C, Cs...>(_value, {});
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    const auto types = std::vector<ValidationType>(
        {C::template to_schema<T>(), Cs::template to_schema<T>()...});
    return ValidationType{ValidationType::AnyOf{.types_ = types}};
  }

 private:
  static std::string make_error_message(const std::vector<Error>& _errors) {
    std::stringstream stream;
    stream << "Expected at least one of the following validations to pass, but "
              "none of them did:";
    for (size_t i = 0; i < _errors.size(); ++i) {
      stream << "\n" << i + 1 << ") " << _errors.at(i).what();
    }
    return stream.str();
  }

  template <class T, class Head, class... Tail>
  static rfl::Result<T> validate_impl(const T& _value,
                                      std::vector<Error> _errors) {
    const auto handle_err = [&](Error&& _err) -> rfl::Result<T> {
      _errors.push_back(std::forward<Error>(_err));
      if constexpr (sizeof...(Tail) == 0) {
        return error(make_error_message(_errors));
      } else {
        return validate_impl<T, Tail...>(
            _value, std::forward<std::vector<Error>>(_errors));
      }
    };
    return Head::validate(_value).or_else(handle_err);
  }
};

}  // namespace rfl

#endif
