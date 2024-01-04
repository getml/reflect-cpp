#ifndef RFL_PARSING_PARSER_DEFAULT_HPP_
#define RFL_PARSING_PARSER_DEFAULT_HPP_

#include <stdexcept>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../from_named_tuple.hpp"
#include "../internal/enums/StringConverter.hpp"
#include "../internal/has_reflection_method_v.hpp"
#include "../internal/has_reflection_type_v.hpp"
#include "../internal/is_basic_type.hpp"
#include "../internal/to_ptr_named_tuple.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

/// Default case - anything that cannot be explicitly matched.
template <class R, class W, class T>
requires AreReaderAndWriter<R, W, T>
struct Parser {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  using ParentType = Parent<W>;

  /// Expresses the variables as type T.
  static Result<T> read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (R::template has_custom_constructor<T>) {
      return _r.template use_custom_constructor<T>(_var);
    } else {
      if constexpr (internal::has_reflection_type_v<T>) {
        using ReflectionType = std::remove_cvref_t<typename T::ReflectionType>;
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
        return _r.template to_basic_type<std::remove_cvref_t<T>>(_var);
      } else {
        static_assert(
            always_false_v<T>,
            "Unsupported type. Please refer to the sections on custom "
            "classes and custom parsers for information on how add "
            "support for your own classes.");
      }
    }
  }

  template <class P>
  static void write(const W& _w, const T& _var, const P& _parent) noexcept {
    if constexpr (internal::has_reflection_type_v<T>) {
      using ReflectionType = std::remove_cvref_t<typename T::ReflectionType>;
      if constexpr (internal::has_reflection_method_v<T>) {
        Parser<R, W, ReflectionType>::write(_w, _var.reflection(), _parent);
      } else {
        const auto& [r] = _var;
        Parser<R, W, ReflectionType>::write(_w, r, _parent);
      }
    } else if constexpr (std::is_class_v<T> && std::is_aggregate_v<T>) {
      const auto ptr_named_tuple = internal::to_ptr_named_tuple(_var);
      using PtrNamedTupleType = std::remove_cvref_t<decltype(ptr_named_tuple)>;
      Parser<R, W, PtrNamedTupleType>::write(_w, ptr_named_tuple, _parent);
    } else if constexpr (std::is_enum_v<T>) {
      using StringConverter = internal::enums::StringConverter<T>;
      const auto str = StringConverter::enum_to_string(_var);
      ParentType::add_value(_w, str, _parent);
    } else if constexpr (internal::is_basic_type_v<T>) {
      ParentType::add_value(_w, _var, _parent);
    } else {
      static_assert(always_false_v<T>,
                    "Unsupported type. Please refer to the sections on custom "
                    "classes and custom parsers for information on how add "
                    "support for your own classes.");
    }
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
