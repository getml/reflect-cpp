#ifndef RFL_INTERNAL_TUP_TO_PTR_TUPLE_HPP_
#define RFL_INTERNAL_TUP_TO_PTR_TUPLE_HPP_

#include <tuple>

#include "../Field.hpp"
#include "../make_named_tuple.hpp"

namespace rfl {
namespace internal {

/// Generates a named tuple that contains pointers to the original values in
/// the struct from a tuple.
template <class TupleType, class... AlreadyExtracted>
auto tup_to_ptr_tuple(TupleType& _t, AlreadyExtracted... _a) {
  constexpr auto i = sizeof...(AlreadyExtracted);
  constexpr auto size = std::tuple_size_v<TupleType>;

  if constexpr (i == size) {
    return std::make_tuple(_a...);
  } else {
    return tup_to_ptr_tuple(_t, _a..., &std::get<i>(_t));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
