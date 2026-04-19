#ifndef RFL_INTERNAL_NOOPTIONALS_HPP_
#define RFL_INTERNAL_NOOPTIONALS_HPP_

#include <type_traits>

#include "../NoOptionals.hpp"

namespace rfl ::internal {

template <class T>
class no_optionals;

template <class T>
class no_optionals : public std::false_type {};

template <>
class no_optionals<NoOptionals> : public std::true_type {};

template <class Head, class... Tail>
struct no_optionals<Processors<Head, Tail...>> {
  static constexpr bool value =
      (no_optionals<Head>::value || ... || no_optionals<Tail>::value);
};

template <class T>
constexpr bool no_optionals_v =
    no_optionals<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
