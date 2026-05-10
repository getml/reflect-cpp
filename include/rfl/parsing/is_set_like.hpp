#ifndef RFL_PARSING_IS_SET_LIKE_HPP_
#define RFL_PARSING_IS_SET_LIKE_HPP_

#include <set>
#include <type_traits>
#include <unordered_set>

namespace rfl {
namespace parsing {

/**
 * @brief Trait to check if a type is a set-like type.
 *
 * @tparam T The type to check.
 */
template <class T>
class is_set_like;

template <class T>
class is_set_like : public std::false_type {};

template <class T>
class is_set_like<std::set<T>> : public std::true_type {};

template <class T>
class is_set_like<std::unordered_set<T>> : public std::true_type {};

template <class T>
class is_set_like<std::multiset<T>> : public std::true_type {};

template <class T>
class is_set_like<std::unordered_multiset<T>> : public std::true_type {};

template <class T>
constexpr bool is_set_like_v =
    is_set_like<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace parsing

}  // namespace rfl

#endif
