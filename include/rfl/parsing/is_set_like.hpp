#ifndef RFL_PARSING_IS_SET_LIKE_HPP_
#define RFL_PARSING_IS_SET_LIKE_HPP_

#include <set>
#include <type_traits>
#include <unordered_set>

namespace rfl {
namespace parsing {

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

}  // namespace parsing
}  // namespace rfl

#endif
