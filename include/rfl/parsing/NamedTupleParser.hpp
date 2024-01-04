#ifndef RFL_PARSING_NAMEDTUPLEPARSER_HPP_
#define RFL_PARSING_NAMEDTUPLEPARSER_HPP_

#include <tuple>
#include <type_traits>
#include <unordered_map>

#include "../NamedTuple.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/Memoization.hpp"
#include "../internal/is_attribute.hpp"
#include "../internal/is_basic_type.hpp"
#include "../internal/strings/replace_all.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "is_empty.hpp"
#include "is_required.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, bool _ignore_empty_containers, class... FieldTypes>
requires AreReaderAndWriter<R, W, NamedTuple<FieldTypes...>>
struct NamedTupleParser {
  using InputObjectType = typename R::InputObjectType;
  using InputVarType = typename R::InputVarType;

  using OutputObjectType = typename W::OutputObjectType;
  using OutputVarType = typename W::OutputVarType;

  using ParentType = Parent<W>;

 public:
  static Result<NamedTuple<FieldTypes...>> read(
      const R& _r, const InputVarType& _var) noexcept {
    const auto& indices = field_indices();
    const auto fct = [&](const std::string_view& _str) -> std::int16_t {
      const auto it = indices.find(_str);
      return it != indices.end() ? it->second : static_cast<std::uint16_t>(-1);
    };
    const auto to_fields_array = [&](auto _obj) {
      return _r.template to_fields_array<sizeof...(FieldTypes)>(fct, _obj);
    };
    const auto build = [&](auto _fields_vec) {
      return build_named_tuple_recursively(_r, _fields_vec);
    };
    return _r.to_object(_var).transform(to_fields_array).and_then(build);
  }

  template <class P>
  static void write(const W& _w, const NamedTuple<FieldTypes...>& _tup,
                    const P& _parent) noexcept {
    auto obj = ParentType::add_object(_w, _tup.size(), _parent);
    build_object_recursively(_w, _tup, &obj);
    _w.end_object(&obj);
  }

 private:
  /// Builds the named tuple field by field.
  template <class... Args>
  static Result<NamedTuple<FieldTypes...>> build_named_tuple_recursively(
      const R& _r,
      const std::array<std::optional<InputVarType>, sizeof...(FieldTypes)>&
          _fields_arr,
      Args&&... _args) noexcept {
    constexpr auto i = sizeof...(Args);

    if constexpr (i == sizeof...(FieldTypes)) {
      return NamedTuple<FieldTypes...>(std::move(_args)...);
    } else {
      using FieldType = typename std::tuple_element<
          i, typename NamedTuple<FieldTypes...>::Fields>::type;

      using ValueType = std::remove_cvref_t<typename FieldType::Type>;

      const auto& f = std::get<i>(_fields_arr);

      if (!f) {
        if constexpr (is_required<ValueType, _ignore_empty_containers>()) {
          const auto key = FieldType::name_.str();
          return collect_errors<i + 1>(
              _r, _fields_arr,
              std::vector<Error>(
                  {Error("Field named '" + key + "' not found!")}));
        } else {
          return build_named_tuple_recursively(
              _r, _fields_arr, std::move(_args)..., FieldType(ValueType()));
        }
      }

      const auto build = [&](auto&& _value) {
        return build_named_tuple_recursively(
            _r, _fields_arr, std::move(_args)..., FieldType(std::move(_value)));
      };

      const auto handle_error = [&](Error&& _error) {
        return collect_errors<i + 1>(_r, _fields_arr,
                                     std::vector<Error>({std::move(_error)}));
      };

      return get_value<FieldType>(_r, *f).or_else(handle_error).and_then(build);
    }
  }

  /// If something went wrong, we want to collect all of the errors - it's
  /// just good UX.
  template <int _i>
  static Error collect_errors(
      const R& _r,
      const std::array<std::optional<InputVarType>, sizeof...(FieldTypes)>&
          _fields_arr,
      std::vector<Error> _errors) noexcept {
    if constexpr (_i == sizeof...(FieldTypes)) {
      if (_errors.size() == 1) {
        return std::move(_errors[0]);
      } else {
        std::string msg =
            "Found " + std::to_string(_errors.size()) + " errors:";
        for (size_t i = 0; i < _errors.size(); ++i) {
          msg += "\n" + std::to_string(i + 1) + ") " +
                 internal::strings::replace_all(_errors.at(i).what(), "\n",
                                                "\n    ");
        }
        return Error(msg);
      }
    } else {
      using FieldType = typename std::tuple_element<
          _i, typename NamedTuple<FieldTypes...>::Fields>::type;

      using ValueType = std::remove_cvref_t<typename FieldType::Type>;

      const auto& f = std::get<_i>(_fields_arr);

      if (!f) {
        if constexpr (is_required<ValueType, _ignore_empty_containers>()) {
          const auto key = FieldType::name_.str();
          _errors.emplace_back(Error("Field named '" + key + "' not found."));
        }
        return collect_errors<_i + 1>(_r, _fields_arr, std::move(_errors));
      }

      const auto add_error_if_applicable =
          [&](Error&& _error) -> Result<ValueType> {
        _errors.emplace_back(std::move(_error));
        return _error;
      };

      get_value<FieldType>(_r, *f).or_else(add_error_if_applicable);

      return collect_errors<_i + 1>(_r, _fields_arr, std::move(_errors));
    }
  }

  /// Builds the object field by field.
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
      const auto name = FieldType::name_.str();
      const auto new_parent = typename ParentType::Object{name, _ptr};
      if constexpr (!is_required<ValueType, _ignore_empty_containers>()) {
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

  /// Uses a memoization pattern to retrieve the field indices.
  /// There are some objects that we are likely to parse many times,
  /// so we only calculate these indices once.
  static const auto& field_indices() noexcept {
    return field_indices_.value(set_field_indices<0>);
  }

  /// Retrieves the value from the object. This is mainly needed to
  /// generate a better error message.
  template <class FieldType>
  static auto get_value(const R& _r, const InputVarType _var) noexcept {
    static_assert(FieldType::name_ != internal::StringLiteral("xml_content") ||
                      internal::is_basic_type_v<typename FieldType::Type>,
                  "xml_content must be a string, a boolean, a floating point "
                  "value or integral.");
    const auto embellish_error = [&](const Error& _e) {
      const auto key = FieldType::name_.str();
      return Error("Failed to parse field '" + key + "': " + _e.what());
    };
    using ValueType = std::remove_cvref_t<typename FieldType::Type>;
    return Parser<R, W, ValueType>::read(_r, _var).or_else(embellish_error);
  }

  /// Builds the object field by field.
  template <size_t _i = 0>
  static void set_field_indices(
      std::unordered_map<std::string_view, std::int16_t>*
          _field_indices) noexcept {
    if constexpr (_i >= sizeof...(FieldTypes)) {
      return;
    } else {
      using FieldType =
          typename std::tuple_element<_i, std::tuple<FieldTypes...>>::type;
      const auto name = FieldType::name_.string_view();
      (*_field_indices)[name] = static_cast<std::int16_t>(_i);
      set_field_indices<_i + 1>(_field_indices);
    }
  }

 private:
  /// Maps each of the field names to an index signifying their order.
  static inline internal::Memoization<
      std::unordered_map<std::string_view, std::int16_t>>
      field_indices_;
};

}  // namespace parsing
}  // namespace rfl

#endif
