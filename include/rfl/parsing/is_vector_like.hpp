#ifndef RFL_PARSING_IS_VECTOR_LIKE_HPP_
#define RFL_PARSING_IS_VECTOR_LIKE_HPP_

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace rfl::parsing {

/**
 * @brief Trait to check if a type is a vector-like type.
 *
 * @tparam T The type to check.
 */
template <class T>
class is_vector_like;

template <class T>
class is_vector_like : public std::false_type {};

template <class T>
class is_vector_like<std::deque<T>> : public std::true_type {};

template <class T>
class is_vector_like<std::forward_list<T>> : public std::true_type {};

template <class T>
class is_vector_like<std::list<T>> : public std::true_type {};

template <class K, class V, class Compare, class Allocator>
class is_vector_like<std::map<K, V, Compare, Allocator>>
    : public std::true_type {};

template <class K, class V, class Compare, class Allocator>
class is_vector_like<std::multimap<K, V, Compare, Allocator>>
    : public std::true_type {};

template <class T, class Compare, class Allocator>
class is_vector_like<std::multiset<T, Compare, Allocator>>
    : public std::true_type {};

template <class T, class Compare, class Allocator>
class is_vector_like<std::set<T, Compare, Allocator>> : public std::true_type {
};

template <class K, class V, class Hash, class KeyEqual, class Allocator>
class is_vector_like<std::unordered_map<K, V, Hash, KeyEqual, Allocator>>
    : public std::true_type {};

template <class K, class V, class Hash, class KeyEqual, class Allocator>
class is_vector_like<std::unordered_multimap<K, V, Hash, KeyEqual, Allocator>>
    : public std::true_type {};

template <class T, class Hash, class KeyEqual, class Allocator>
class is_vector_like<std::unordered_multiset<T, Hash, KeyEqual, Allocator>>
    : public std::true_type {};

template <class T, class Hash, class KeyEqual, class Allocator>
class is_vector_like<std::unordered_set<T, Hash, KeyEqual, Allocator>>
    : public std::true_type {};

template <class T>
class is_vector_like<std::vector<T>> : public std::true_type {};

template <class T>
constexpr bool is_vector_like_v = is_vector_like<std::remove_cvref_t<T>>::value;

}  // namespace rfl::parsing

#endif
