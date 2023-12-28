#ifndef RFL_PARSING_ISWRITER_HPP_
#define RFL_PARSING_ISWRITER_HPP_

#include <concepts>

#include "../Result.hpp"

namespace rfl {
namespace parsing {

template <class W, class T>
concept IsWriter = requires(W w, T t, std::string name,
                            typename W::OutputArrayType arr,
                            typename W::OutputObjectType obj,
                            typename W::OutputVarType var, size_t size) {
  { w.array_as_root(size) } -> std::same_as<typename W::OutputArrayType>;

  { w.object_as_root(size) } -> std::same_as<typename W::OutputObjectType>;

  { w.null_as_root() } -> std::same_as<typename W::OutputVarType>;

  { w.value_as_root(name) } -> std::same_as<typename W::OutputVarType>;

  {
    w.add_array_to_array(size, &arr)
    } -> std::same_as<typename W::OutputArrayType>;

  {
    w.add_object_to_array(size, &arr)
    } -> std::same_as<typename W::OutputObjectType>;

  {
    w.add_array_to_object(name, size, &obj)
    } -> std::same_as<typename W::OutputArrayType>;

  {
    w.add_object_to_object(name, size, &obj)
    } -> std::same_as<typename W::OutputObjectType>;

  {
    w.add_value_to_array(name, &arr)
    } -> std::same_as<typename W::OutputVarType>;

  {
    w.add_value_to_object(name, name, &obj)
    } -> std::same_as<typename W::OutputVarType>;

  { w.add_null_to_array(&arr) } -> std::same_as<typename W::OutputVarType>;

  {
    w.add_null_to_object(name, &obj)
    } -> std::same_as<typename W::OutputVarType>;

  { w.end_array(&arr) } -> std::same_as<void>;

  { w.end_object(&obj) } -> std::same_as<void>;
};

}  // namespace parsing
}  // namespace rfl

#endif
