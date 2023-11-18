#ifndef RFL_PARSING_IS_MAP_LIKE_NOT_MULTIMAP_HPP_
#define RFL_PARSING_IS_MAP_LIKE_NOT_MULTIMAP_HPP_

#include <map>
#include <type_traits>
#include <unordered_map>

namespace rfl {
namespace parsing {

template <class T>
class is_map_like_not_multimap;

template <class T>
class is_map_like_not_multimap : public std::false_type {};

template <class K, class V>
class is_map_like_not_multimap<std::map<K, V>> : public std::true_type {};

template <class K, class V>
class is_map_like_not_multimap<std::unordered_map<K, V>>
    : public std::true_type {};

}  // namespace parsing
}  // namespace rfl

#endif
