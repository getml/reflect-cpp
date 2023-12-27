#ifndef RFL_PARSING_ISWRITER_HPP_
#define RFL_PARSING_ISWRITER_HPP_

#include <concepts>

#include "rfl/Result.hpp"

namespace rfl {
namespace parsing {

template <class W, class T>
concept IsWriter = requires(W w, T t, std::string name,
                            typename W::OutputArrayType arr,
                            typename W::OutputObjectType obj,
                            typename W::OutputVarType var) {
  { w.end_array(&arr) } -> std::same_as<void>;
};

}  // namespace parsing
}  // namespace rfl

#endif
