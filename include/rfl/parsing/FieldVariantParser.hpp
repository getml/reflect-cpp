#ifndef RFL_PARSING_FIELD_VARIANT_PARSER_HPP_
#define RFL_PARSING_FIELD_VARIANT_PARSER_HPP_

#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

/// To be used when all options of the variants are rfl::Field. Essentially,
/// this is an externally tagged union.
template <class R, class W, class... FieldTypes>
requires AreReaderAndWriter<R, W, std::variant<FieldTypes...>>
struct FieldVariantParser {
  using ResultType = Result<std::variant<FieldTypes...>>;

 public:
  using InputObjectType = typename R::InputObjectType;
  using InputVarType = typename R::InputVarType;

  using OutputObjectType = typename W::OutputObjectType;
  using OutputVarType = typename W::OutputVarType;

  static ResultType read(const R& _r, const InputVarType& _var) noexcept {
    static_assert(
        internal::no_duplicate_field_names<std::tuple<FieldTypes...>>(),
        "Externally tagged variants cannot have duplicate field "
        "names.");

    const auto to_map = [&](auto _obj) { return _r.to_map(_obj); };

    const auto to_result = [&](auto _map) -> ResultType {
      if (_map.size() != 1) {
        return Error(
            "Could not parse Field: Expected the object to have "
            "exactly one field, but found " +
            std::to_string(_map.size()) + " fields.");
      }
      const auto it = _map.begin();
      const auto& disc_value = it->first;
      const auto& var = it->second;
      return find_matching_alternative(_r, disc_value, var);
    };

    return _r.to_object(_var).transform(to_map).and_then(to_result);
  }

  template <class P>
  static void write(const W& _w, const std::variant<FieldTypes...>& _v,
                    const P& _parent) noexcept {
    static_assert(
        internal::no_duplicate_field_names<std::tuple<FieldTypes...>>(),
        "Externally tagged variants cannot have duplicate field "
        "names.");

    const auto handle = [&](const auto& _field) {
      const auto named_tuple = make_named_tuple(internal::to_ptr_field(_field));
      using NamedTupleType = std::remove_cvref_t<decltype(named_tuple)>;
      Parser<R, W, NamedTupleType>::write(_w, named_tuple, _parent);
    };

    std::visit(handle, _v);
  }

 private:
  template <int _i = 0>
  static ResultType find_matching_alternative(
      const R& _r, const std::string& _disc_value,
      const InputVarType& _var) noexcept {
    if constexpr (_i == sizeof...(FieldTypes)) {
      return Error(
          "Could not parse std::variant, could not match field named "
          "'" +
          _disc_value + "'.");
    } else {
      using FieldType = std::remove_cvref_t<
          typename std::tuple_element<_i, std::tuple<FieldTypes...>>::type>;

      using ValueType = std::remove_cvref_t<typename FieldType::Type>;

      const auto key = FieldType::name_.str();

      if (key == _disc_value) {
        const auto to_variant = [](ValueType&& _val) {
          return std::variant<FieldTypes...>(FieldType(std::move(_val)));
        };

        const auto embellish_error = [&](const Error& _e) {
          return Error("Could not parse std::variant with field '" +
                       _disc_value + "': " + _e.what());
        };

        return Parser<R, W, ValueType>::read(_r, _var)
            .transform(to_variant)
            .or_else(embellish_error);
      } else {
        return find_matching_alternative<_i + 1>(_r, _disc_value, _var);
      }
    }
  }
};
}  // namespace parsing
}  // namespace rfl

#endif
