#ifndef RFL_PARSING_PARSER_ENUM_HPP_
#define RFL_PARSING_PARSER_ENUM_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../enums.hpp"
#include "../thirdparty/enchantum/enchantum.hpp"
#include "../internal/has_reflector.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "call_destructors_where_necessary.hpp"
#include "make_type_name.hpp"
#include "schema/Type.hpp"
#include "schemaful/IsSchemafulReader.hpp"
#include "schemaful/IsSchemafulWriter.hpp"

namespace rfl::parsing {

/// Default case - anything that cannot be explicitly matched.
template <class R, class W, class T, class ProcessorsType>
  requires std::is_enum_v<T> && AreReaderAndWriter<R, W, T>
struct Parser<R, W, T, ProcessorsType> {
 public:
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /// Expresses the variables as type T.
  static Result<T> read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (internal::has_read_reflector<T>) {
      const auto wrap_in_t = [](auto&& _named_tuple) -> Result<T> {
        try {
          using NT = decltype(_named_tuple);
          return Reflector<T>::to(std::forward<NT>(_named_tuple));
        } catch (std::exception& e) {
          return error(e.what());
        }
      };
      return Parser<R, W, typename Reflector<T>::ReflType,
                    ProcessorsType>::read(_r, _var)
          .and_then(wrap_in_t);

    } else if constexpr (ProcessorsType::underlying_enums_ ||
                  schemaful::IsSchemafulReader<R>) {
      static_assert(enchantum::ScopedEnum<T>,
                    "The enum must be a scoped enum in order to retrieve "
                    "the underlying value.");
      return _r.template to_basic_type<std::underlying_type_t<T>>(_var)
          .transform([](const auto _val) { return static_cast<T>(_val); });
    } else {
      return _r.template to_basic_type<std::string>(_var).and_then(
          rfl::string_to_enum<T>);
    }
  }

  template <class P>
  static void write(const W& _w, const T& _var, const P& _parent) {
    if constexpr (internal::has_write_reflector<T>) {
      Parser<R, W, typename Reflector<T>::ReflType, ProcessorsType>::write(
          _w, Reflector<T>::from(_var), _parent);
    } else if constexpr (ProcessorsType::underlying_enums_ ||
                  schemaful::IsSchemafulWriter<W>) {
      const auto val = static_cast<std::underlying_type_t<T>>(_var);
      ParentType::add_value(_w, val, _parent);
    } else {
      const auto str = rfl::enum_to_string(_var);
      ParentType::add_value(_w, str, _parent);
    }
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    if constexpr (internal::has_read_reflector<U> ||
                         internal::has_write_reflector<U>) {
      return make_reference<U>(_definitions);
    } else {
      return make_enum<U>(_definitions);
    }    
  }

 private:
  template <class U>
  static schema::Type make_enum(
      std::map<std::string, schema::Type>* _definitions) {
    using Type = schema::Type;
    if constexpr (ProcessorsType::underlying_enums_ ||
                  schemaful::IsSchemafulReader<R>) {
      return Type{Type::Integer{}};
    } else if constexpr (enchantum::is_bitflag<U>) {
      return Type{Type::String{}};
    } else {
      return Parser<
          R, W,
          typename decltype(internal::enums::get_enum_names<U>())::Literal,
          ProcessorsType>::to_schema(_definitions);
    }
  }

  template <class U>
  static schema::Type make_reference(
      std::map<std::string, schema::Type>* _definitions) {
    using Type = schema::Type;
    const auto name = make_type_name<U>();

    if (_definitions->find(name) == _definitions->end()) {
        (*_definitions)[name] = Parser<R, W, typename Reflector<U>::ReflType,
                                       ProcessorsType>::to_schema(_definitions);
    }
    return Type{Type::Reference{name}};
  }
};

}  // namespace rfl::parsing

#endif
