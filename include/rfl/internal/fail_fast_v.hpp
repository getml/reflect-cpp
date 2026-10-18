#ifndef RFL_INTERNAL_FAILFAST_HPP_
#define RFL_INTERNAL_FAILFAST_HPP_

#include <type_traits>

#include "../FailFast.hpp"
#include "../Processors.hpp"

namespace rfl::internal {

template <class T>
struct fail_fast : std::false_type {};

template <>
struct fail_fast<FailFast> : std::true_type {};

template <class... Ps>
struct fail_fast<Processors<Ps...>>
    : std::bool_constant<(fail_fast<Ps>::value || ...)> {};

template <class T>
inline constexpr bool fail_fast_v = fail_fast<std::remove_cvref_t<T>>::value;

}  // namespace rfl::internal

#endif
