#ifndef RFL_INTERNAL_PROCESSED_T_HPP_
#define RFL_INTERNAL_PROCESSED_T_HPP_

#include <type_traits>

#include "../Processors.hpp"
#include "../named_tuple_t.hpp"

namespace rfl::internal {

template <class StructType, class ProcessorsType>
struct Processed;

template <class StructType, class... Ps>
struct Processed<StructType, Processors<Ps...>> {
  using NamedTupleType = named_tuple_t<StructType>;
  using type = typename std::invoke_result<
      decltype(Processors<Ps...>::template process<StructType, NamedTupleType>),
      NamedTupleType>::type;
};

template <class StructType, class ProcessorsType>
using processed_t = typename Processed<StructType, ProcessorsType>::type;

}  // namespace rfl::internal

#endif
