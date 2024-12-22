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
  /// A schemaful writer needs to know two additional types:
  /// 1) Maps - unlike objects, their field names are not known at compile time.
  /// 2) Unions - schemaful formats need explicit union types.

  /// Sets an empty map as the root element of the document.
  /// Some serialization formats require you to pass the expected size in
  /// advance. If you are not working with such a format, you can ignore the
  /// parameter `size`. Returns the new array for further modification.
  { w.map_as_root(size) } -> std::same_as<typename W::OutputMapType>;

  /// Sets an empty union as the root element of the document.
  { w.union_as_root() } -> std::same_as<typename W::OutputUnionType>;

  /// Adds an empty array to an existing map. Returns the new
  /// array for further modification.
  {
    w.add_array_to_map(name, size, &m)
  } -> std::same_as<typename W::OutputArrayType>;

  /// Adds an empty array to an existing union.
  /// The index refers to the index of the element in the union.
  /// Returns the new array for further modification.
  {
    w.add_array_to_union(index, size, &u)
  } -> std::same_as<typename W::OutputArrayType>;

  /// Adds an empty map to an existing array. Returns the new
  /// map for further modification.
  { w.add_map_to_array(size, &arr) } -> std::same_as<typename W::OutputMapType>;

  /// Adds an empty map to an existing map. The key or name of the field
  /// is signified by `name`. Returns the new map for further modification.
  {
    w.add_map_to_map(name, size, &m)
  } -> std::same_as<typename W::OutputMapType>;

  /// Adds an empty map to an existing object. The key or name of the field
  /// is signified by `name`. Returns the new map for further modification.
  {
    w.add_map_to_object(name, size, &obj)
  } -> std::same_as<typename W::OutputMapType>;

  /// Adds an empty map to an existing union.
  /// The index refers to the index of the element in the union.
  /// Returns the new map for further modification.
  {
    w.add_map_to_union(index, size, &u)
  } -> std::same_as<typename W::OutputMapType>;

  /// Adds an empty object to an existing map. The key or name of the field
  /// is signified by `name`. Returns the new object for further modification.
  {
    w.add_object_to_map(name, size, &m)
  } -> std::same_as<typename W::OutputObjectType>;

  /// Adds an empty object to an existing union.
  /// The index refers to the index of the element in the union.
  /// Returns the new object for further modification.
  {
    w.add_object_to_union(index, size, &u)
  } -> std::same_as<typename W::OutputObjectType>;

  /// Adds an empty union to an existing array. Returns the new
  /// union for further modification.
  { w.add_union_to_array(&arr) } -> std::same_as<typename W::OutputUnionType>;

  /// Adds an empty union to an existing map. The key or name of the field
  /// is signified by `name`. Returns the new union for further modification.
  { w.add_union_to_map(name, &m) } -> std::same_as<typename W::OutputUnionType>;

  /// Adds an empty union to an existing object. The key or name of the field
  /// is signified by `name`. Returns the new union for further modification.
  {
    w.add_union_to_object(name, &obj)
  } -> std::same_as<typename W::OutputUnionType>;

  /// Adds an empty union to an existing union.
  /// The index refers to the index of the element in the union.
  /// Returns the new union for further modification.
  {
    w.add_union_to_union(index, &u)
  } -> std::same_as<typename W::OutputUnionType>;

  /// Adds a null value to a map. Returns an
  /// OutputVarType containing the null value.
  { w.add_null_to_map(name, &m) } -> std::same_as<typename W::OutputVarType>;

  /// Adds a basic value (bool, numeric, string) to an existing map. The key
  /// or name of the field is signified by `name`. Returns an
  /// OutputVarType containing the new value.
  {
    w.add_value_to_map(name, val, &m)
  } -> std::same_as<typename W::OutputVarType>;

  /// Adds a null value to a union. Returns an
  /// OutputVarType containing the null value.
  { w.add_null_to_union(index, &u) } -> std::same_as<typename W::OutputVarType>;

  /// Adds a basic value (bool, numeric, string) to an existing union. The key
  /// or name of the field is signified by `name`. Returns an
  /// OutputVarType containing the new value.
  {
    w.add_value_to_union(index, val, &u)
  } -> std::same_as<typename W::OutputVarType>;

  /// Signifies to the writer that we do not want to add any further elements to
  /// this map. Some serialization formats require this. If you are working
  /// with a serialization format that doesn't, just leave the function empty.
  { w.end_map(&m) } -> std::same_as<void>;
};

}  // namespace rfl::parsing::schemaful

#endif
