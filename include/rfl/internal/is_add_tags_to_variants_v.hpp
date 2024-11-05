#ifndef RFL_INTERNAL_ISADDTAGSTOVARIANTS_HPP_
#define RFL_INTERNAL_ISADDTAGSTOVARIANTS_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

#include "../AddTagsToVariants.hpp"

namespace rfl ::internal {

template <class T>
class is_add_tags_to_variants;

template <class T>
class is_add_tags_to_variants : public std::false_type {};

template <>
class is_add_tags_to_variants<AddTagsToVariants> : public std::true_type {};

template <class T>
constexpr bool is_add_tags_to_variants_v = is_add_tags_to_variants<
    std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
