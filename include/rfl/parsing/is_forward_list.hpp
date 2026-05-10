#ifndef RFL_PARSING_IS_FORWARD_LIST_HPP_
#define RFL_PARSING_IS_FORWARD_LIST_HPP_

#include <forward_list>
#include <type_traits>

namespace rfl {
namespace parsing {

/**
 * @brief Trait to check if a type is a std::forward_list.
 *
 * @tparam T The type to check.
 */
template <class T>
class is_forward_list;

template <class T>
class is_forward_list : public std::false_type {};

template <class T>
class is_forward_list<std::forward_list<T>> : public std::true_type {};

}  // namespace parsing
}  // namespace rfl

#endif
