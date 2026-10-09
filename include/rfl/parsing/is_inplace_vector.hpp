#ifndef RFL_PARSING_IS_INPLACE_VECTOR_HPP_
#define RFL_PARSING_IS_INPLACE_VECTOR_HPP_

#include <cstddef>
#include <type_traits>

#if __has_include(<inplace_vector>)
#include <inplace_vector>
#endif

namespace rfl {
namespace parsing {

/**
 * @brief Trait to check if a type is a std::inplace_vector.
 *
 * @tparam T The type to check.
 */
template <class T>
class is_inplace_vector;

template <class T>
class is_inplace_vector : public std::false_type {};

#if defined(__cpp_lib_inplace_vector)
template <class T, std::size_t N>
class is_inplace_vector<std::inplace_vector<T, N>> : public std::true_type {};
#endif  // __cpp_lib_inplace_vector

template <class T>
constexpr bool is_inplace_vector_v =
    is_inplace_vector<std::remove_cvref_t<T>>::value;

}  // namespace parsing
}  // namespace rfl

#endif
