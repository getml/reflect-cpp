#ifndef RFL_PARSING_ISWRITER_HPP_
#define RFL_PARSING_ISWRITER_HPP_

#include <concepts>

#include "rfl/Result.hpp"

namespace rfl {
namespace parsing {

template <class W>
concept IsWriter = requires(W w, std::string name,
                            typename W::OutputArrayType arr,
                            typename W::OutputObjectType obj,
                            typename W::OutputVarType var) {
    { w.add(var, &arr) } -> std::same_as<void>;

    { w.empty_var() } -> std::same_as<typename W::OutputVarType>;

    { w.new_array() } -> std::same_as<typename W::OutputArrayType>;

    { w.new_object() } -> std::same_as<typename W::OutputObjectType>;

    { w.is_empty(&var) } -> std::same_as<bool>;

    { w.set_field(name, var, &obj) } -> std::same_as<void>;
};

}  // namespace parsing
}  // namespace rfl

#endif
