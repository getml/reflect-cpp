#ifndef RFL_PARSING_IS_STRING_MAP_HPP_
#define RFL_PARSING_IS_STRING_MAP_HPP_

#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "../ExtraFields.hpp"
#include "../Object.hpp"

namespace rfl ::parsing {

template <class T>
struct is_string_map : std::false_type {};

template <class T, class Compare, class Allocator>
struct is_string_map<std::map<std::string, T, Compare, Allocator>>
    : std::true_type {};

template <class T, class Hash, class KeyEqual, class Allocator>
struct is_string_map<
    std::unordered_map<std::string, T, Hash, KeyEqual, Allocator>>
    : std::true_type {};

template <class T>
struct is_string_map<rfl::Object<T>> : std::true_type {};

template <class T>
struct is_string_map<rfl::ExtraFields<T>> : std::true_type {};

template <class T>
constexpr bool is_string_map_v = is_string_map<std::remove_cvref_t<T>>::value;

}  // namespace rfl::parsing

#endif
