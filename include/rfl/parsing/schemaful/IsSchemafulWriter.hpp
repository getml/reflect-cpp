#ifndef RFL_PARSING_SCHEMAFUL_ISSCHEMAFULWRITER_HPP_
#define RFL_PARSING_SCHEMAFUL_ISSCHEMAFULWRITER_HPP_

#include <concepts>
#include <string>
#include <string_view>
#include <variant>

namespace rfl::parsing::schemaful {

template <class W>
concept IsSchemafulWriter = requires(
    W w, typename W::OutputVarType var, typename W::OutputArrayType arr,
    typename W::OutputObjectType obj, typename W::OutputUnionType u,
    size_t index, std::string_view name) {
  {
    w.add_array_to_union(index, index, &u)
  } -> std::same_as<typename W::OutputArrayType>;

  {
    w.add_object_to_union(index, index, &u)
  } -> std::same_as<typename W::OutputObjectType>;

  { w.add_union_to_array(&arr) } -> std::same_as<typename W::OutputUnionType>;

  {
    w.add_union_to_object(name, &obj)
  } -> std::same_as<typename W::OutputUnionType>;

  {
    w.add_union_to_union(index, &u)
  } -> std::same_as<typename W::OutputUnionType>;

  { w.add_null_to_union(index, &u) } -> std::same_as<typename W::OutputVarType>;

  {
    w.add_value_to_union(index, index, &u)
  } -> std::same_as<typename W::OutputVarType>;
};

}  // namespace rfl::parsing::schemaful

#endif
