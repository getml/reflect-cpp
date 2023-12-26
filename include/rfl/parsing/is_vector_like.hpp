#ifndef RFL_PARSING_IS_VECTOR_LIKE_HPP_
#define RFL_PARSING_IS_VECTOR_LIKE_HPP_

#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace rfl {
namespace parsing {

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

template <class T>
class is_vector_like<std::multiset<T>> : public std::true_type {};

template <class T>
class is_vector_like<std::set<T>> : public std::true_type {};

template <class T>
class is_vector_like<std::unordered_multiset<T>> : public std::true_type {};

template <class T>
class is_vector_like<std::unordered_set<T>> : public std::true_type {};

template <class T>
class is_vector_like<std::vector<T>> : public std::true_type {};

template <class T>
constexpr bool is_vector_like_v =
    is_vector_like<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace parsing
}  // namespace rfl

#endif
