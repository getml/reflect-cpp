#ifndef RFL_PARSING_NAMEDTUPLEPARSER_HPP_
#define RFL_PARSING_NAMEDTUPLEPARSER_HPP_

#include <array>
#include <map>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "../NamedTuple.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/is_array.hpp"
#include "../internal/is_attribute.hpp"
#include "../internal/is_basic_type.hpp"
#include "../internal/is_extra_fields.hpp"
#include "../internal/is_skip.hpp"
#include "../internal/no_duplicate_field_names.hpp"
#include "../internal/nth_element_t.hpp"
#include "../internal/ptr_cast.hpp"
#include "../to_view.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "ViewReader.hpp"
#include "ViewReaderWithDefault.hpp"
#include "ViewReaderWithDefaultAndStrippedFieldNames.hpp"
#include "ViewReaderWithStrippedFieldNames.hpp"
#include "call_destructors_where_necessary.hpp"
#include "is_empty.hpp"
#include "is_required.hpp"
#include "schema/Type.hpp"
#include "to_single_error_message.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, bool _ignore_empty_containers, bool _all_required,
          bool _no_field_names, class ProcessorsType, class... FieldTypes>
  requires AreReaderAndWriter<R, W, NamedTuple<FieldTypes...>>
struct NamedTupleParser {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  using NamedTupleType = NamedTuple<FieldTypes...>;

  using ViewReaderType = std::conditional_t<
      _no_field_names,
      ViewReaderWithStrippedFieldNames<R, W, NamedTupleType, ProcessorsType>,
      ViewReader<R, W, NamedTupleType, ProcessorsType>>;

  using ViewReaderWithDefaultType = std::conditional_t<
      _no_field_names,
      ViewReaderWithDefaultAndStrippedFieldNames<R, W, NamedTupleType,
                                                 ProcessorsType>,
      ViewReaderWithDefault<R, W, NamedTupleType, ProcessorsType>>;

  using InputObjectOrArrayType =
      std::conditional_t<_no_field_names, typename R::InputArrayType,
                         typename R::InputObjectType>;
  using OutputObjectOrArrayType =
      std::conditional_t<_no_field_names, typename W::OutputArrayType,
                         typename W::OutputObjectType>;

  using SchemaType = std::conditional_t<_no_field_names, schema::Type::Tuple,
                                        schema::Type::Object>;

  static constexpr size_t size_ = NamedTupleType::size();

  static_assert(NamedTupleType::pos_extra_fields() == -1 || !_no_field_names,
                "You cannot use the rfl::NoFieldNames processor if you are "
                "including rfl::ExtraFields.");

 public:
  /// The way this works is that we allocate space on the stack in this size of
  /// the named tuple in which we then write the individual fields using
  /// views and placement new. This is how we deal with the fact that some
  /// fields might not be default-constructible.
  static Result<NamedTuple<FieldTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    static_assert(
        internal::no_duplicate_field_names<typename NamedTupleType::Fields>());
    alignas(NamedTuple<FieldTypes...>) unsigned char
        buf[sizeof(NamedTuple<FieldTypes...>)];
    auto ptr = internal::ptr_cast<NamedTuple<FieldTypes...>*>(&buf);
    auto view = rfl::to_view(*ptr);
    using ViewType = std::remove_cvref_t<decltype(view)>;
    const auto [set, err] =
        Parser<R, W, ViewType, ProcessorsType>::read_view(_r, _var, &view);
    if (err) [[unlikely]] {
      call_destructors_where_necessary(set, &view);
      return error(*err);
    }
    auto res = Result<NamedTuple<FieldTypes...>>(std::move(*ptr));
    call_destructors_where_necessary(set, &view);
    return res;
  }

  /// Reads the data into a view assuming no default values.
  static std::pair<std::array<bool, NamedTupleType::size()>,
                   std::optional<Error>>
  read_view(const R& _r, const InputVarType& _var,
            NamedTuple<FieldTypes...>* _view) noexcept {
    static_assert(
        internal::no_duplicate_field_names<typename NamedTupleType::Fields>());
    if constexpr (_no_field_names) {
      auto arr = _r.to_array(_var);
      if (!arr) [[unlikely]] {
        auto set = std::array<bool, NamedTupleType::size()>{};
        // return std::make_pair(set, arr.error());
        return std::make_pair(set, arr.error());
      }
      return read_object_or_array(_r, *arr, _view);
    } else {
      auto obj = _r.to_object(_var);
      if (!obj) [[unlikely]] {
        auto set = std::array<bool, NamedTupleType::size()>{};
        return std::make_pair(set, obj.error());
      }
      return read_object_or_array(_r, *obj, _view);
    }
  }

  /// Reads the data into a view assuming default values.
  static std::optional<Error> read_view_with_default(
      const R& _r, const InputVarType& _var,
      NamedTuple<FieldTypes...>* _view) noexcept {
    static_assert(
        internal::no_duplicate_field_names<typename NamedTupleType::Fields>());
    if constexpr (_no_field_names) {
      auto arr = _r.to_array(_var);
      if (!arr) [[unlikely]] {
        return arr.error();
      }
      return read_object_or_array_with_default(_r, *arr, _view);
    } else {
      auto obj = _r.to_object(_var);
      if (!obj) [[unlikely]] {
        return obj.error();
      }
      return read_object_or_array_with_default(_r, *obj, _view);
    }
  }

  template <class P>
  static void write(const W& _w, const NamedTuple<FieldTypes...>& _tup,
                    const P& _parent) noexcept {
    if constexpr (_no_field_names) {
      auto arr = ParentType::add_array(_w, _tup.num_fields(), _parent);
      build_object(_w, _tup, &arr, std::make_integer_sequence<int, size_>());
      _w.end_array(&arr);
    } else {
      auto obj = ParentType::add_object(_w, _tup.num_fields(), _parent);
      build_object(_w, _tup, &obj, std::make_integer_sequence<int, size_>());
      _w.end_object(&obj);
    }
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) noexcept {
    SchemaType schema;
    build_schema(_definitions, &schema,
                 std::make_integer_sequence<int, size_>());
    return schema::Type{schema};
  }

 private:
  template <int _i>
  static void add_field_to_object(const W& _w,
                                  const NamedTuple<FieldTypes...>& _tup,
                                  OutputObjectOrArrayType* _ptr) noexcept {
    using FieldType = internal::nth_element_t<_i, FieldTypes...>;
    using ValueType = std::remove_cvref_t<typename FieldType::Type>;
    const auto value = rfl::get<_i>(_tup);
    if constexpr (internal::is_extra_fields_v<ValueType>) {
      for (const auto& [k, v] : *value) {
        const auto new_parent = make_parent(k, _ptr);
        Parser<R, W, std::remove_cvref_t<decltype(v)>, ProcessorsType>::write(
            _w, v, new_parent);
      }
    } else if constexpr (!_all_required && !_no_field_names &&
                         !is_required<ValueType, _ignore_empty_containers>()) {
      constexpr auto name = FieldType::name_.string_view();
      const auto new_parent = make_parent(name, _ptr);
      if (!is_empty(value)) {
        if constexpr (internal::is_attribute_v<ValueType>) {
          Parser<R, W, ValueType, ProcessorsType>::write(
              _w, value, new_parent.as_attribute());
        } else {
          Parser<R, W, ValueType, ProcessorsType>::write(_w, value, new_parent);
        }
      }
    } else {
      constexpr auto name = FieldType::name_.string_view();
      const auto new_parent = make_parent(name, _ptr);
      if constexpr (internal::is_attribute_v<ValueType>) {
        Parser<R, W, ValueType, ProcessorsType>::write(
            _w, value, new_parent.as_attribute());
      } else {
        Parser<R, W, ValueType, ProcessorsType>::write(_w, value, new_parent);
      }
    }
  }

  template <size_t _i>
  static void add_field_to_schema(
      std::map<std::string, schema::Type>* _definitions,
      SchemaType* _schema) noexcept {
    using F = internal::nth_element_t<_i, FieldTypes...>;
    using U = std::remove_cvref_t<typename F::Type>;
    if constexpr (!internal::is_skip_v<U> && !internal::is_extra_fields_v<U>) {
      auto s = Parser<R, W, U, ProcessorsType>::to_schema(_definitions);
      if constexpr (_no_field_names) {
        _schema->types_.emplace_back(std::move(s));
      } else {
        _schema->types_[std::string(F::name())] = std::move(s);
      }
    }
  };

  template <int... _is>
  static void build_object(const W& _w, const NamedTuple<FieldTypes...>& _tup,
                           OutputObjectOrArrayType* _ptr,
                           std::integer_sequence<int, _is...>) noexcept {
    (add_field_to_object<_is>(_w, _tup, _ptr), ...);
  }

  template <int... _is>
  static void build_schema(std::map<std::string, schema::Type>* _definitions,
                           SchemaType* _schema,
                           std::integer_sequence<int, _is...>) noexcept {
    (add_field_to_schema<_is>(_definitions, _schema), ...);

    if constexpr (NamedTupleType::pos_extra_fields() != -1) {
      using F = internal::nth_element_t<NamedTupleType::pos_extra_fields(),
                                        FieldTypes...>;
      using ExtraFieldsType = std::remove_cvref_t<typename F::Type>;
      using U = std::remove_cvref_t<typename ExtraFieldsType::Type>;
      _schema->additional_properties_ = std::make_shared<schema::Type>(
          Parser<R, W, U, ProcessorsType>::to_schema(_definitions));
    }
  }

  /// Generates error messages for when fields are missing.
  template <int _i>
  static void handle_one_missing_field(const std::array<bool, size_>& _found,
                                       const NamedTupleType& _view,
                                       std::array<bool, size_>* _set,
                                       std::vector<Error>* _errors) noexcept {
    using FieldType = internal::nth_element_t<_i, FieldTypes...>;
    using ValueType = std::remove_reference_t<
        std::remove_pointer_t<typename FieldType::Type>>;

    if (!std::get<_i>(_found)) {
      constexpr bool is_required_field =
          !internal::is_extra_fields_v<ValueType> &&
          (_all_required || is_required<ValueType, _ignore_empty_containers>());
      if constexpr (is_required_field) {
        constexpr auto current_name =
            internal::nth_element_t<_i, FieldTypes...>::name();
        std::stringstream stream;
        stream << "Field named '" << std::string(current_name)
               << "' not found.";
        _errors->emplace_back(Error(stream.str()));
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
  }

  /// Generates error messages for when fields are missing.
  template <int... _is>
  static void handle_missing_fields(
      const std::array<bool, size_>& _found, const NamedTupleType& _view,
      std::array<bool, size_>* _set, std::vector<Error>* _errors,
      std::integer_sequence<int, _is...>) noexcept {
    (handle_one_missing_field<_is>(_found, _view, _set, _errors), ...);
  }

  static auto make_parent(const std::string_view& _name,
                          OutputObjectOrArrayType* _ptr) {
    if constexpr (_no_field_names) {
      return typename ParentType::Array{_ptr};
    } else {
      return typename ParentType::Object{_name, _ptr};
    }
  }

  static std::pair<std::array<bool, NamedTupleType::size()>,
                   std::optional<Error>>
  read_object_or_array(const R& _r, const InputObjectOrArrayType& _obj_or_arr,
                       NamedTupleType* _view) noexcept {
    auto found = std::array<bool, NamedTupleType::size()>();
    found.fill(false);
    auto set = std::array<bool, NamedTupleType::size()>();
    set.fill(false);
    std::vector<Error> errors;
    const auto reader = ViewReaderType(&_r, _view, &found, &set, &errors);
    std::optional<Error> err;
    if constexpr (_no_field_names) {
      err = _r.read_array(reader, _obj_or_arr);
    } else {
      err = _r.read_object(reader, _obj_or_arr);
    }
    if (err) {
      return std::make_pair(set, err);
    }
    handle_missing_fields(found, *_view, &set, &errors,
                          std::make_integer_sequence<int, size_>());
    if (errors.size() != 0) {
      return std::make_pair(set, to_single_error_message(errors));
    }
    return std::make_pair(set, std::optional<Error>());
  }

  static std::optional<Error> read_object_or_array_with_default(
      const R& _r, const InputObjectOrArrayType& _obj_or_arr,
      NamedTupleType* _view) noexcept {
    std::vector<Error> errors;
    const auto reader = ViewReaderWithDefaultType(&_r, _view, &errors);
    std::optional<Error> err;
    if constexpr (_no_field_names) {
      err = _r.read_array(reader, _obj_or_arr);
    } else {
      err = _r.read_object(reader, _obj_or_arr);
    }
    if (err) {
      return err;
    }
    if (errors.size() != 0) {
      return to_single_error_message(errors);
    }
    return std::nullopt;
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
