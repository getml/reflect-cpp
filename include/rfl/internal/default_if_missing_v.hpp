#ifndef RFL_INTERNAL_DEFAULTIFMISSING_HPP_
#define RFL_INTERNAL_DEFAULTIFMISSING_HPP_

#include <type_traits>

#include "../DefaultIfMissing.hpp"
#include "../Processors.hpp"

namespace rfl::internal {

template <class T>
class default_if_missing;

template <class T>
class default_if_missing : public std::false_type {};

template <>
class default_if_missing<DefaultIfMissing> : public std::true_type {};

template <class Head, class... Tail>
struct default_if_missing<Processors<Head, Tail...>> {
  static constexpr bool value = (default_if_missing<Head>::value || ... ||
                                 default_if_missing<Tail>::value);
};

template <class T>
constexpr bool default_if_missing_v =
    default_if_missing<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
