#ifndef RFL_INTERNAL_TAG_T_HPP_
#define RFL_INTERNAL_TAG_T_HPP_

#include <type_traits>

#include "make_tag.hpp"

namespace rfl {
namespace internal {

template <class T>
using tag_t = typename std::invoke_result<decltype(make_tag<T>)>::type;

}  // namespace internal
}  // namespace rfl

#endif
