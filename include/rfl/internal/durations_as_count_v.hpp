#ifndef RFL_INTERNAL_DURATIONSASCOUNT_HPP_
#define RFL_INTERNAL_DURATIONSASCOUNT_HPP_

#include <type_traits>

#include "../Processors.hpp"
#include "../DurationsAsCount.hpp"

namespace rfl::internal {

template <class T>
class durations_as_count;

template <class T>
class durations_as_count : public std::false_type {};

template <>
class durations_as_count<DurationsAsCount> : public std::true_type {};

template <class Head, class... Tail>
struct durations_as_count<Processors<Head, Tail...>> {
  static constexpr bool value =
      (durations_as_count<Head>::value || ... || durations_as_count<Tail>::value);
};

template <class T>
constexpr bool durations_as_count_v =
    durations_as_count<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
