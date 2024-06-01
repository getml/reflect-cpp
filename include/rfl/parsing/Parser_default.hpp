#ifndef RFL_PARSING_PARSER_DEFAULT_HPP_
#define RFL_PARSING_PARSER_DEFAULT_HPP_

#include <map>
#include <stdexcept>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../from_named_tuple.hpp"
#include "../internal/enums/StringConverter.hpp"
#include "../internal/has_reflection_method_v.hpp"
#include "../internal/has_reflection_type_v.hpp"
#include "../internal/is_basic_type.hpp"
#include "../internal/is_description.hpp"
#include "../internal/is_literal.hpp"
#include "../internal/is_validator.hpp"
#include "../internal/processed_t.hpp"
#include "../internal/to_ptr_named_tuple.hpp"
#include "../to_view.hpp"
#include "../type_name_t.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "is_tagged_union_wrapper.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

/// Default case - anything that cannot be explicitly matched.
template <class R, class W, class T, class ProcessorsType>
requires AreReaderAndWriter<R, W, T>
struct Parser {
 public:
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
        return Parser<R, W, ReflectionType, ProcessorsType>::read(_r, _var)
            .and_then(wrap_in_t);
      } else if constexpr (std::is_class_v<T> && std::is_aggregate_v<T>) {
        return read_struct(_r, _var);
      } else if constexpr (std::is_enum_v<T>) {
        using StringConverter = internal::enums::StringConverter<T>;
        return _r.template to_basic_type<std::string>(_var).and_then(
            StringConverter::string_to_enum);
      } else {
        return _r.template to_basic_type<std::remove_cvref_t<T>>(_var);
      }
    }
  }

  template <class P>
  static void write(const W& _w, const T& _var, const P& _parent) noexcept {
    if constexpr (internal::has_reflection_type_v<T>) {
      using ReflectionType = std::remove_cvref_t<typename T::ReflectionType>;
      if constexpr (internal::has_reflection_method_v<T>) {
        Parser<R, W, ReflectionType, ProcessorsType>::write(
            _w, _var.reflection(), _parent);
      } else {
        const auto& [r] = _var;
        Parser<R, W, ReflectionType, ProcessorsType>::write(_w, r, _parent);
      }
    } else if constexpr (std::is_class_v<T> && std::is_aggregate_v<T>) {
      const auto ptr_named_tuple = ProcessorsType::template process<T>(
          internal::to_ptr_named_tuple(_var));
      using PtrNamedTupleType = std::remove_cvref_t<decltype(ptr_named_tuple)>;
      Parser<R, W, PtrNamedTupleType, ProcessorsType>::write(
          _w, ptr_named_tuple, _parent);
    } else if constexpr (std::is_enum_v<T>) {
      using StringConverter = internal::enums::StringConverter<T>;
      const auto str = StringConverter::enum_to_string(_var);
      ParentType::add_value(_w, str, _parent);
    } else {
      ParentType::add_value(_w, _var, _parent);
    }
  }

  /// Generates a schema for the underlying type.
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    using Type = schema::Type;
    if constexpr (std::is_same<U, bool>()) {
      return Type{Type::Boolean{}};

    } else if constexpr (std::is_same<U, std::int32_t>()) {
      return Type{Type::Int32{}};

    } else if constexpr (std::is_same<U, std::int64_t>()) {
      return Type{Type::Int64{}};

    } else if constexpr (std::is_same<U, std::uint32_t>()) {
      return Type{Type::UInt32{}};

    } else if constexpr (std::is_same<U, std::uint64_t>()) {
      return Type{Type::UInt64{}};

    } else if constexpr (std::is_integral<U>()) {
      return Type{Type::Integer{}};

    } else if constexpr (std::is_same<U, float>()) {
      return Type{Type::Float{}};

    } else if constexpr (std::is_floating_point_v<U>) {
      return Type{Type::Double{}};

    } else if constexpr (std::is_same<U, std::string>()) {
      return Type{Type::String{}};

    } else if constexpr (rfl::internal::is_description_v<U>) {
      return make_description<U>(_definitions);

    } else if constexpr (std::is_enum_v<U>) {
      return make_enum<U>(_definitions);

    } else if constexpr (std::is_class_v<U> && std::is_aggregate_v<U>) {
      return make_reference<U>(_definitions);

    } else if constexpr (internal::is_literal_v<U>) {
      return Type{Type::Literal{.values_ = U::strings()}};

    } else if constexpr (internal::is_validator_v<U>) {
      return make_validated<U>(_definitions);

    } else if constexpr (internal::has_reflection_type_v<U>) {
      return Parser<R, W, typename U::ReflectionType,
                    ProcessorsType>::to_schema(_definitions);

    } else {
      static_assert(rfl::always_false_v<U>, "Unsupported type.");
    }
  }

 private:
  template <class U>
  static schema::Type make_description(
      std::map<std::string, schema::Type>* _definitions) {
    using Type = schema::Type;
    return Type{Type::Description{
        .description_ = typename U::Content().str(),
        .type_ =
            Ref<Type>::make(Parser<R, W, std::remove_cvref_t<typename U::Type>,
                                   ProcessorsType>::to_schema(_definitions))}};
  }

  template <class U>
  static schema::Type make_enum(
      std::map<std::string, schema::Type>* _definitions) {
    using Type = schema::Type;
    using S = internal::enums::StringConverter<U>;
    if constexpr (S::is_flag_enum_) {
      return Type{Type::String{}};
    } else {
      return Parser<R, W, typename S::NamesLiteral, ProcessorsType>::to_schema(
          _definitions);
    }
  }

  template <class U>
  static schema::Type make_reference(
      std::map<std::string, schema::Type>* _definitions) {
    using Type = schema::Type;
    const auto name = make_type_name<U>();
    if (_definitions->find(name) == _definitions->end()) {
      (*_definitions)[name] =
          Type{Type::Integer{}};  // Placeholder to avoid infinite loop.
      using NamedTupleType = internal::processed_t<U, ProcessorsType>;
      (*_definitions)[name] =
          Parser<R, W, NamedTupleType, ProcessorsType>::to_schema(_definitions);
    }
    return Type{Type::Reference{name}};
  }

  template <class U>
  static schema::Type make_validated(
      std::map<std::string, schema::Type>* _definitions) {
    using Type = schema::Type;
    using ReflectionType = std::remove_cvref_t<typename U::ReflectionType>;
    using ValidationType = std::remove_cvref_t<typename U::ValidationType>;
    return Type{Type::Validated{
        .type_ = Ref<Type>::make(
            Parser<R, W, ReflectionType, ProcessorsType>::to_schema(
                _definitions)),
        .validation_ = ValidationType::template to_schema<ReflectionType>()}};
  }

  template <class U>
  static std::string make_type_name() {
    if constexpr (is_tagged_union_wrapper_v<U>) {
      return replace_non_alphanumeric(type_name_t<typename U::Type>().str() +
                                      "__tagged");
    } else {
      return replace_non_alphanumeric(type_name_t<U>().str());
    }
  }

  /// The way this works is that we allocate space on the stack in this size of
  /// the struct in which we then write the individual fields using
  /// views and placement new. This is how we deal with the fact that some
  /// fields might not be default-constructible.
  static Result<T> read_struct(const R& _r, const InputVarType& _var) {
    alignas(T) unsigned char buf[sizeof(T)];
    auto ptr = reinterpret_cast<T*>(buf);
    auto view = ProcessorsType::template process<T>(to_view(*ptr));
    using ViewType = std::remove_cvref_t<decltype(view)>;
    const auto err =
        Parser<R, W, ViewType, ProcessorsType>::read_view(_r, _var, &view);
    if (err) [[unlikely]] {
      return *err;
    }
    return std::move(*ptr);
  }

  static std::string replace_non_alphanumeric(std::string _str) {
    for (auto& ch : _str) {
      ch = std::isalnum(ch) ? ch : '_';
    }
    return _str;
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
