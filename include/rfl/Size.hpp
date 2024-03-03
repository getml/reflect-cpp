#ifndef RFL_SIZE_HPP_
#define RFL_SIZE_HPP_

#include <map>

#include "Ref.hpp"
#include "Result.hpp"
#include "parsing/Parser.hpp"
#include "parsing/schema/ValidationType.hpp"

namespace rfl {

template <class V>
struct Size {
  template <class T>
  static rfl::Result<T> validate(const T& _t) {
    const auto to_t = [&](const auto& _v) { return _t; };
    const auto embellish_error = [](const auto& _err) {
      return Error("Size validation failed: " + _err.what());
    };
    return V::validate(_t.size()).transform(to_t).or_else(embellish_error);
  }

  template <class T>
  static parsing::schema::ValidationType to_schema() {
    return V::template to_schema<size_t>();
  }
};

}  // namespace rfl

#endif
