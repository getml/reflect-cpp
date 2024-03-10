#ifndef RFL_PARSING_ISWRITER_HPP_
#define RFL_PARSING_ISWRITER_HPP_

#include <concepts>
#include <string>
#include <string_view>

#include "../Result.hpp"

namespace rfl {
namespace parsing {

template <class W, class T>
concept IsWriter = requires(W w, T t, std::string_view name,
                            std::string basic_value,
                            typename W::OutputArrayType arr,
                            typename W::OutputObjectType obj,
                            typename W::OutputVarType var, size_t size) {
  /// Sets an empty array as the root element of the document.
  /// Some serialization formats require you to pass the expected size in
  /// advance. If you are not working with such a format, you can ignore the
  /// parameter `size`. Returns the new array for further modification.
  { w.array_as_root(size) } -> std::same_as<typename W::OutputArrayType>;

  /// Sets an empty object as the root element of the document.
  /// Some serialization formats require you to pass the expected size in
  /// advance. If you are not working with such a format, you can ignore the
  /// parameter `size`.
  /// Returns the new object for further modification.
  { w.object_as_root(size) } -> std::same_as<typename W::OutputObjectType>;

  /// Sets a null as the root element of the document. Returns OutputVarType
  /// containing the null value.
  { w.null_as_root() } -> std::same_as<typename W::OutputVarType>;

  /// Sets a basic value (bool, numeric, string) as the root element of the
  /// document. Returns an OutputVarType containing the new value.
  { w.value_as_root(basic_value) } -> std::same_as<typename W::OutputVarType>;

  /// Adds an empty array to an existing array. Returns the new
  /// array for further modification.
  {
    w.add_array_to_array(size, &arr)
    } -> std::same_as<typename W::OutputArrayType>;

  /// Adds an empty object to an existing array. Returns the new
  /// object for further modification.
  {
    w.add_object_to_array(size, &arr)
    } -> std::same_as<typename W::OutputObjectType>;

  /// Adds an empty array to an existing object. The key or name of the field is
  /// signified by `name`. Returns the new array for further modification.
  {
    w.add_array_to_object(name, size, &obj)
    } -> std::same_as<typename W::OutputArrayType>;

  /// Adds an empty object to an existing object. The key or name of the field
  /// is signified by `name`. Returns the new object for further modification.
  {
    w.add_object_to_object(name, size, &obj)
    } -> std::same_as<typename W::OutputObjectType>;

  /// Adds a basic value (bool, numeric, string) to an array. Returns an
  /// OutputVarType containing the new value.
  {
    w.add_value_to_array(basic_value, &arr)
    } -> std::same_as<typename W::OutputVarType>;

  /// Adds a basic value (bool, numeric, string) to an existing object. The key
  /// or name of the field is signified by `name`. Returns an
  /// OutputVarType containing the new value.
  {
    w.add_value_to_object(name, basic_value, &obj)
    } -> std::same_as<typename W::OutputVarType>;

  /// Adds a null value to an array. Returns an
  /// OutputVarType containing the null value.
  { w.add_null_to_array(&arr) } -> std::same_as<typename W::OutputVarType>;

  /// Adds a null value to an existing object. The key
  /// or name of the field is signified by `name`. Returns an
  /// OutputVarType containing the null value.
  {
    w.add_null_to_object(name, &obj)
    } -> std::same_as<typename W::OutputVarType>;

  /// Signifies to the writer that we do not want to add any further elements to
  /// this array. Some serialization formats require this. If you are working
  /// with a serialization format that doesn't, just leave the function empty.
  { w.end_array(&arr) } -> std::same_as<void>;

  /// Signifies to the writer that we do not want to add any further elements to
  /// this object. Some serialization formats require this. If you are working
  /// with a serialization format that doesn't, just leave the function empty.
  { w.end_object(&obj) } -> std::same_as<void>;
};

}  // namespace parsing
}  // namespace rfl

#endif
