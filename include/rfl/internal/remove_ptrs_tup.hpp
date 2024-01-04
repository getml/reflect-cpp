#ifndef RFL_INTERNAL_REMOVE_PTRS_TUP_HPP_
#define RFL_INTERNAL_REMOVE_PTRS_TUP_HPP_

#include <functional>
#include <tuple>
#include <type_traits>

#include "ptr_tuple_t.hpp"
#include "../to_named_tuple.hpp"

namespace rfl {
namespace internal {

template <class T>
struct remove_ptrs_tup;

template <class... Ts>
struct remove_ptrs_tup<std::tuple<Ts...>> {
  using TupleType =
      std::tuple<std::remove_cvref_t<std::remove_pointer_t<Ts>>...>;
};

}  // namespace internal
}  // namespace rfl

#endif
