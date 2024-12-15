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
    typename W::OutputMapType m, typename W::OutputObjectType obj,
    typename W::OutputUnionType u, size_t index, size_t size,
    std::string_view name, std::string val) {
  { w.map_as_root(size) } -> std::same_as<typename W::OutputMapType>;

  { w.union_as_root() } -> std::same_as<typename W::OutputUnionType>;

  {
    w.add_array_to_map(name, size, &m)
  } -> std::same_as<typename W::OutputArrayType>;

  {
    w.add_array_to_union(index, size, &u)
  } -> std::same_as<typename W::OutputArrayType>;

  { w.add_map_to_array(size, &arr) } -> std::same_as<typename W::OutputMapType>;

  {
    w.add_map_to_map(name, size, &m)
  } -> std::same_as<typename W::OutputMapType>;

  {
    w.add_map_to_object(name, size, &obj)
  } -> std::same_as<typename W::OutputMapType>;

  {
    w.add_map_to_union(index, size, &u)
  } -> std::same_as<typename W::OutputMapType>;

  {
    w.add_object_to_map(name, size, &m)
  } -> std::same_as<typename W::OutputObjectType>;

  {
    w.add_object_to_union(index, size, &u)
  } -> std::same_as<typename W::OutputObjectType>;

  { w.add_union_to_array(&arr) } -> std::same_as<typename W::OutputUnionType>;

  { w.add_union_to_map(name, &m) } -> std::same_as<typename W::OutputUnionType>;

  {
    w.add_union_to_object(name, &obj)
  } -> std::same_as<typename W::OutputUnionType>;

  {
    w.add_union_to_union(index, &u)
  } -> std::same_as<typename W::OutputUnionType>;

  { w.add_null_to_map(name, &m) } -> std::same_as<typename W::OutputVarType>;

  {
    w.add_value_to_map(name, val, &m)
  } -> std::same_as<typename W::OutputVarType>;

  { w.add_null_to_union(index, &u) } -> std::same_as<typename W::OutputVarType>;

  {
    w.add_value_to_union(index, val, &u)
  } -> std::same_as<typename W::OutputVarType>;

  { w.end_map(&m) } -> std::same_as<void>;
};

}  // namespace rfl::parsing::schemaful

#endif
