#ifndef RFL_PARSING_NAMEDTUPLEPARSER_HPP_
#define RFL_PARSING_NAMEDTUPLEPARSER_HPP_

#include <array>
#include <map>
#include <tuple>
#include <type_traits>
#include <unordered_map>

#include "../NamedTuple.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/Memoization.hpp"
#include "../internal/is_array.hpp"
#include "../internal/is_attribute.hpp"
#include "../internal/is_basic_type.hpp"
#include "../internal/strings/replace_all.hpp"
#include "../to_view.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "ViewReader.hpp"
#include "is_empty.hpp"
#include "is_required.hpp"
#include "schema/Type.hpp"
#include "to_single_error_message.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, bool _ignore_empty_containers, bool _all_required,
          class... FieldTypes>
requires AreReaderAndWriter<R, W, NamedTuple<FieldTypes...>>
struct NamedTupleParser {
  using InputObjectType = typename R::InputObjectType;
  using InputVarType = typename R::InputVarType;

  using OutputObjectType = typename W::OutputObjectType;
  using OutputVarType = typename W::OutputVarType;

  using ParentType = Parent<W>;

  using NamedTupleType = NamedTuple<FieldTypes...>;

  static constexpr size_t size_ = NamedTupleType::size();

 public:
  /// The way this works is that we allocate space on the stack in this size of
  /// the named tuple in which we then write the individual fields using
  /// views and placement new. This is how we deal with the fact that some
  /// fields might not be default-constructible.
  static Result<NamedTuple<FieldTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    alignas(NamedTuple<FieldTypes...>) unsigned char
        buf[sizeof(NamedTuple<FieldTypes...>)];
    auto ptr = reinterpret_cast<NamedTuple<FieldTypes...>*>(buf);
    const auto view = rfl::to_view(*ptr);
    using ViewType = std::remove_cvref_t<decltype(view)>;
    const auto err = Parser<R, W, ViewType>::read_view(_r, _var, view);
    if (err) {
      return *err;
    }
    return *ptr;
  }

  /// Reads the data into a view.
  static std::optional<Error> read_view(
      const R& _r, const InputVarType& _var,
      const NamedTuple<FieldTypes...>& _view) noexcept {
    const auto obj = _r.to_object(_var);
    if (obj) {
      return read_object(_r, *obj, _view);
    }
    return obj.error();
  }

  /// For writing, we do not need to make the distinction between
  /// default-constructible and non-default constructible fields.
  template <class P>
  static void write(const W& _w, const NamedTuple<FieldTypes...>& _tup,
                    const P& _parent) noexcept {
    auto obj = ParentType::add_object(_w, _tup.size(), _parent);
    build_object_recursively(_w, _tup, &obj);
    _w.end_object(&obj);
  }

  /// For generating the schema, we also do not need to make the distinction
  /// between default-constructible and non-default constructible fields.
  template <size_t _i = 0>
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions,
      std::map<std::string, schema::Type> _values = {}) {
    using Type = schema::Type;
    using T = NamedTuple<FieldTypes...>;
    constexpr size_t size = T::size();
    if constexpr (_i == size) {
      return Type{Type::Object{_values}};
    } else {
      using F = std::tuple_element_t<_i, typename T::Fields>;
      _values[std::string(F::name())] =
          Parser<R, W, typename F::Type>::to_schema(_definitions);
      return to_schema<_i + 1>(_definitions, _values);
    }
  };

 private:
  template <int _i = 0>
  static void build_object_recursively(const W& _w,
                                       const NamedTuple<FieldTypes...>& _tup,
                                       OutputObjectType* _ptr) noexcept {
    if constexpr (_i >= sizeof...(FieldTypes)) {
      return;
    } else {
      using FieldType =
          typename std::tuple_element<_i, std::tuple<FieldTypes...>>::type;
      using ValueType = std::remove_cvref_t<typename FieldType::Type>;
      const auto& value = rfl::get<_i>(_tup);
      constexpr auto name = FieldType::name_.string_view();
      const auto new_parent = typename ParentType::Object{name, _ptr};
      if constexpr (!_all_required &&
                    !is_required<ValueType, _ignore_empty_containers>()) {
        if (!is_empty(value)) {
          if constexpr (internal::is_attribute_v<ValueType>) {
            Parser<R, W, ValueType>::write(_w, value,
                                           new_parent.as_attribute());
          } else {
            Parser<R, W, ValueType>::write(_w, value, new_parent);
          }
        }
      } else {
        if constexpr (internal::is_attribute_v<ValueType>) {
          Parser<R, W, ValueType>::write(_w, value, new_parent.as_attribute());
        } else {
          Parser<R, W, ValueType>::write(_w, value, new_parent);
        }
      }
      return build_object_recursively<_i + 1>(_w, _tup, _ptr);
    }
  }

  /// C-arrays need special handling here.
  template <class T>
  static void call_destructor_on_array(const size_t _size, T* _ptr) {
    for (size_t i = 0; i < _size; ++i) {
      if constexpr (std::is_array_v<T>) {
        call_destructor_on_array(sizeof(*_ptr) / sizeof(**_ptr), *(_ptr + i));
      } else if constexpr (std::is_destructible_v<T>) {
        (_ptr + i)->~T();
      }
    }
  }

  /// Because of the way we have allocated the fields, we need to manually
  /// trigger the destructors.
  template <size_t _i = 0>
  static void call_destructors_where_necessary(
      const NamedTupleType& _view, const std::array<bool, size_>& _set) {
    if constexpr (_i < sizeof...(FieldTypes)) {
      using FieldType =
          std::tuple_element_t<_i, typename NamedTupleType::Fields>;
      using ValueType =
          std::remove_cvref_t<std::remove_pointer_t<typename FieldType::Type>>;
      if constexpr (!std::is_array_v<ValueType> &&
                    std::is_destructible_v<ValueType>) {
        if (std::get<_i>(_set)) {
          rfl::get<_i>(_view)->~ValueType();
        }
      } else if constexpr (std::is_array_v<ValueType>) {
        if (std::get<_i>(_set)) {
          auto ptr = rfl::get<_i>(_view);
          call_destructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
        }
      }
      call_destructors_where_necessary<_i + 1>(_view, _set);
    }
  }

  /// Generates error messages for when fields are missing.
  template <size_t _i = 0>
  static void handle_missing_fields(const std::array<bool, size_>& _found,
                                    const NamedTupleType& _view,
                                    std::array<bool, size_>* _set,
                                    std::vector<Error>* _errors) noexcept {
    if constexpr (_i < sizeof...(FieldTypes)) {
      using FieldType =
          std::tuple_element_t<_i, typename NamedTupleType::Fields>;
      using ValueType = std::remove_reference_t<
          std::remove_pointer_t<typename FieldType::Type>>;

      if (!std::get<_i>(_found)) {
        if constexpr (_all_required ||
                      is_required<ValueType, _ignore_empty_containers>()) {
          constexpr auto current_name =
              std::tuple_element_t<_i, typename NamedTupleType::Fields>::name();
          _errors->push_back("Field named '" + std::string(current_name) +
                             "' not found.");
        } else {
          if constexpr (!std::is_const_v<ValueType>) {
            ::new (rfl::get<_i>(_view)) ValueType();
          } else {
            using NonConstT = std::remove_const_t<ValueType>;
            ::new (const_cast<NonConstT*>(rfl::get<_i>(_view))) NonConstT();
          }
          std::get<_i>(*_set) = true;
        }
      }
      handle_missing_fields<_i + 1>(_found, _view, _set, _errors);
    }
  }

  static std::optional<Error> read_object(
      const R& _r, const InputObjectType& _obj,
      const NamedTupleType& _view) noexcept {
    auto found = std::array<bool, NamedTupleType::size()>();
    found.fill(false);
    auto set = std::array<bool, NamedTupleType::size()>();
    set.fill(false);
    std::vector<Error> errors;
    const auto object_reader =
        ViewReader<R, W, NamedTupleType>(&_r, &_view, &found, &set, &errors);
    const auto err = _r.read_object(object_reader, _obj);
    if (err) {
      return *err;
    }
    handle_missing_fields(found, _view, &set, &errors);
    if (errors.size() != 0) {
      call_destructors_where_necessary(_view, set);
      return to_single_error_message(errors);
    }
    return std::nullopt;
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
