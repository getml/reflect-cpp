#ifndef RFL_INTERNAL_PROCESSORS_HPP_
#define RFL_INTERNAL_PROCESSORS_HPP_

#include <type_traits>
#include <utility>

#include "Tuple.hpp"

namespace rfl {

/// Represents a collection of processors that transform named tuples sequentially.
/// Processors are applied from left to right, each transforming the output of the previous one.
template <class... Ps>
struct Processors;

/// Base case: no processors, returns the named tuple unchanged.
template <>
struct Processors<> {
  /// Processes a named tuple (a tuple-like structure with named fields) without modification.
  /// @tparam T The type of the struct being processed
  /// @tparam NamedTupleType The type of the named tuple
  /// @param _named_tuple The named tuple to process
  /// @return The unmodified named tuple
  template <class T, class NamedTupleType>
  static auto process(NamedTupleType&& _named_tuple) {
    return std::forward<NamedTupleType>(_named_tuple);
  }
};

/// Recursive case: applies Head processor, then recursively applies Tail processors.
template <class Head, class... Tail>
struct Processors<Head, Tail...> {
  /// Processes a named tuple by applying Head processor then all Tail processors.
  /// @tparam T The type of the struct being processed
  /// @tparam NamedTupleType The type of the named tuple
  /// @param _named_tuple The named tuple to process
  /// @return The transformed named tuple after all processors have been applied
  template <class T, class NamedTupleType>
  static auto process(NamedTupleType&& _named_tuple) {
    return Processors<Tail...>::template process<T>(
        Head::template process<T>(std::move(_named_tuple)));
  }
};

}  // namespace rfl

#endif
