#ifndef RFL_SIZE_HPP_
#define RFL_SIZE_HPP_

#include <map>

#include "Ref.hpp"
#include "Result.hpp"
#include "parsing/schema/ValidationType.hpp"

namespace rfl {

template <class V>
struct Size {
  template <class T>
  static rfl::Result<T> validate(const T& _t) {
    const auto to_t = [&](const auto&) { return _t; };
    const auto embellish_error = [](const auto& _err) -> Error {
      return Error("Size validation failed: " + _err.what());
    };
    return V::validate(_t.size()).transform(to_t).transform_error(
        embellish_error);
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    using ValidationType = parsing::schema::ValidationType;
    return ValidationType{ValidationType::Size{
        .size_limit_ =
            rfl::Ref<ValidationType>::make(V::template to_schema<size_t>())}};
  }
};

}  // namespace rfl

#endif
