#ifndef RFL_PARSING_IS_MAP_LIKE_HPP_
#define RFL_PARSING_IS_MAP_LIKE_HPP_

#include <map>
#include <type_traits>
#include <unordered_map>

namespace rfl {
namespace parsing {

template <class T>
class is_map_like;

template <class T>
class is_map_like : public std::false_type {};

template <class K, class V>
class is_map_like<std::map<K, V>> : public std::true_type {};

template <class K, class V>
class is_map_like<std::multimap<K, V>> : public std::true_type {};

template <class K, class V>
class is_map_like<std::unordered_map<K, V>> : public std::true_type {};

template <class K, class V>
class is_map_like<std::unordered_multimap<K, V>> : public std::true_type {};

template <class T>
constexpr bool is_map_like_v =
    is_map_like<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace parsing
}  // namespace rfl

#endif
