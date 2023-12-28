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
                            typename W::OutputVarType var) {
    /// Any Writer needs to define the following:
    ///
    /// 1) An OutputArrayType, which must be an array-like data structure.
    /// 2) An OutputObjectType, which must contain key-value pairs.
    /// 3) An OutputVarType, which must be able to represent either
    ///    OutputArrayType, OutputObjectType or a basic type (bool, integral,
    ///    floating point, std::string).

    /// Appends var to the end of arr, thus mutating it.
    { w.add(var, &arr) } -> std::same_as<void>;

    /// Returns an empty var (usually the NULL type).
    { w.empty_var() } -> std::same_as<typename W::OutputVarType>;

    /// Generates a var from a basic type (std::string, bool, floating point or
    /// integral).
    { w.from_basic_type(t) } -> std::same_as<typename W::OutputVarType>;

    /// Generates a new, empty array.
    { w.new_array() } -> std::same_as<typename W::OutputArrayType>;

    /// Generates a new, empty object.
    { w.new_object() } -> std::same_as<typename W::OutputObjectType>;

    /// Determines whether the var is empty (the NULL type).
    { w.is_empty(var) } -> std::same_as<bool>;

    /// Adds a new field to obj, thus mutating it.
    { w.set_field(name, var, &obj) } -> std::same_as<void>;
};

}  // namespace parsing
}  // namespace rfl

#endif
