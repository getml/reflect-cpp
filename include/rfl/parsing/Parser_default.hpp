#ifndef RFL_PARSING_PARSER_DEFAULT_HPP_
#define RFL_PARSING_PARSER_DEFAULT_HPP_

#include <stdexcept>
#include <type_traits>

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/from_named_tuple.hpp"
#include "rfl/internal/enums/StringConverter.hpp"
#include "rfl/internal/has_reflection_method_v.hpp"
#include "rfl/internal/has_reflection_type_v.hpp"
#include "rfl/internal/is_attribute.hpp"
#include "rfl/internal/is_basic_type.hpp"
#include "rfl/internal/to_ptr_named_tuple.hpp"
#include "rfl/parsing/AreReaderAndWriter.hpp"
#include "rfl/parsing/Parser_base.hpp"
#include "rfl/parsing/supports_attributes.hpp"

namespace rfl {
namespace parsing {

/// Default case - anything that cannot be explicitly matched.
template <class R, class W, class T>
requires AreReaderAndWriter<R, W, T>
struct Parser {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  /// Expresses the variables as type T.
  static Result<T> read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (R::template has_custom_constructor<T>) {
      return _r.template use_custom_constructor<T>(_var);
    } else {
      if constexpr (internal::has_reflection_type_v<T>) {
        using ReflectionType = std::decay_t<typename T::ReflectionType>;
        const auto wrap_in_t = [](auto _named_tuple) -> Result<T> {
          try {
            return T(_named_tuple);
          } catch (std::exception& e) {
            return Error(e.what());
          }
        };
        return Parser<R, W, ReflectionType>::read(_r, _var).and_then(wrap_in_t);
      } else if constexpr (std::is_class_v<T> && std::is_aggregate_v<T>) {
        using NamedTupleType = named_tuple_t<T>;
        const auto to_struct = [](NamedTupleType&& _n) -> Result<T> {
          try {
            return from_named_tuple<T>(std::move(_n));
          } catch (std::exception& e) {
            return Error(e.what());
          }
        };
        return Parser<R, W, NamedTupleType>::read(_r, _var).and_then(to_struct);
      } else if constexpr (std::is_enum_v<T>) {
        using StringConverter = internal::enums::StringConverter<T>;
        return _r.template to_basic_type<std::string>(_var).and_then(
            StringConverter::string_to_enum);
      } else if constexpr (internal::is_basic_type_v<T>) {
        return _r.template to_basic_type<std::decay_t<T>>(_var);
      } else {
        static_assert(
            always_false_v<T>,
            "Unsupported type. Please refer to the sections on custom "
            "classes and custom parsers for information on how add "
            "support for your own classes.");
      }
    }
  }

  /// Converts the variable to a JSON type.
  static auto write(const W& _w, const T& _var) noexcept {
    if constexpr (supports_attributes<W> && internal::is_attribute_v<T>) {
      const auto r = resolve_reflection_type(_var);
      return _w.from_basic_type(r, true);
    } else if constexpr (internal::has_reflection_type_v<T>) {
      const auto r = resolve_reflection_type(_var);
      return Parser<R, W, std::decay_t<decltype(r)>>::write(_w, r);
    } else if constexpr (std::is_class_v<T> && std::is_aggregate_v<T>) {
      const auto ptr_named_tuple = internal::to_ptr_named_tuple(_var);
      using PtrNamedTupleType = std::decay_t<decltype(ptr_named_tuple)>;
      return Parser<R, W, PtrNamedTupleType>::write(_w, ptr_named_tuple);
    } else if constexpr (std::is_enum_v<T>) {
      using StringConverter = internal::enums::StringConverter<T>;
      const auto str = StringConverter::enum_to_string(_var);
      return _w.from_basic_type(str);
    } else if constexpr (internal::is_basic_type_v<T>) {
      return _w.from_basic_type(_var);
    } else {
      static_assert(always_false_v<T>,
                    "Unsupported type. Please refer to the sections on custom "
                    "classes and custom parsers for information on how add "
                    "support for your own classes.");
    }
  }

  template <class V>
  static const auto resolve_reflection_type(const V& _var) {
    if constexpr (internal::has_reflection_type_v<V>) {
      if constexpr (internal::has_reflection_method_v<V>) {
        return resolve_reflection_type(_var.reflection());
      } else {
        const auto& [r] = _var;
        return resolve_reflection_type(r);
      }
    } else {
      return _var;
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
