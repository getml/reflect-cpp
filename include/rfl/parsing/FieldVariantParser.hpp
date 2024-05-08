#ifndef RFL_PARSING_FIELD_VARIANT_PARSER_HPP_
#define RFL_PARSING_FIELD_VARIANT_PARSER_HPP_

#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "FieldVariantReader.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

/// To be used when all options of the variants are rfl::Field. Essentially,
/// this is an externally tagged union.
template <class R, class W, class ProcessorsType, class... FieldTypes>
requires AreReaderAndWriter<R, W, std::variant<FieldTypes...>>
struct FieldVariantParser {
  using FieldVariantType = std::variant<FieldTypes...>;
  using ResultType = Result<FieldVariantType>;

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

    const auto to_result = [&](const auto _obj) -> ResultType {
      auto field_variant = std::optional<Result<FieldVariantType>>();
      const auto reader =
          FieldVariantReader<R, W, ProcessorsType, FieldTypes...>(
              &_r, &field_variant);
      auto err = _r.read_object(reader, _obj);
      if (err) {
        return *err;
      }
      if (!field_variant) {
        return Error(
            "Could not parse: Expected the object to have "
            "exactly one field, but found more than one.");
      }
      return std::move(*field_variant);
    };

    return _r.to_object(_var).and_then(to_result);
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
      Parser<R, W, NamedTupleType, ProcessorsType>::write(_w, named_tuple,
                                                          _parent);
    };

    std::visit(handle, _v);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions,
      std::vector<schema::Type> _types = {}) {
    using VariantType = std::variant<NamedTuple<FieldTypes>...>;
    return Parser<R, W, VariantType, ProcessorsType>::to_schema(_definitions);
  }
};
}  // namespace parsing
}  // namespace rfl

#endif
