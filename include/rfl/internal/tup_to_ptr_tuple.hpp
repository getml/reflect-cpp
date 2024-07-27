#ifndef RFL_INTERNAL_TUP_TO_PTR_TUPLE_HPP_
#define RFL_INTERNAL_TUP_TO_PTR_TUPLE_HPP_

#include <tuple>

#include "../Field.hpp"
#include "../Tuple.hpp"
#include "../apply.hpp"
#include "../make_named_tuple.hpp"

namespace rfl {
namespace internal {

/// Generates a named tuple that contains pointers to the original values in
/// the struct from a tuple.
template <class TupleType>
auto tup_to_ptr_tuple(TupleType& _t) {
  const auto to_ptr = [](auto&... _fields) {
    return rfl::make_tuple(&_fields...);
  };
  return rfl::apply(to_ptr, _t);
}

}  // namespace internal
}  // namespace rfl

#endif
