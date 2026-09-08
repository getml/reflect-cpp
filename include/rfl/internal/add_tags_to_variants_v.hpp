#ifndef RFL_INTERNAL_ADDTAGSTOVARIANTS_HPP_
#define RFL_INTERNAL_ADDTAGSTOVARIANTS_HPP_

#include <type_traits>

#include "../AddTagsToVariants.hpp"
#include "../Processors.hpp"

namespace rfl::internal {

template <class T>
class add_tags_to_variants;

template <class T>
class add_tags_to_variants : public std::false_type {};

template <>
class add_tags_to_variants<AddTagsToVariants> : public std::true_type {};

template <class Head, class... Tail>
struct add_tags_to_variants<Processors<Head, Tail...>> {
  static constexpr bool value = (add_tags_to_variants<Head>::value || ... ||
                                 add_tags_to_variants<Tail>::value);
};

template <class T>
constexpr bool add_tags_to_variants_v =
    add_tags_to_variants<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

template <class T>
class add_namespaced_tags_to_variants;

template <class T>
class add_namespaced_tags_to_variants : public std::false_type {};

template <>
class add_namespaced_tags_to_variants<AddNamespacedTagsToVariants>
    : public std::true_type {};

template <class Head, class... Tail>
struct add_namespaced_tags_to_variants<Processors<Head, Tail...>> {
  static constexpr bool value =
      (add_namespaced_tags_to_variants<Head>::value || ... ||
       add_namespaced_tags_to_variants<Tail>::value);

  static_assert(
      !add_tags_to_variants<Processors<Head, Tail...>>::value ||
          !add_namespaced_tags_to_variants<Processors<Head, Tail...>>::value,
      "Cannot use both AddTagsToVariants and AddNamespacedTagsToVariants in "
      "the same Processors list.");
};

template <class T>
constexpr bool add_namespaced_tags_to_variants_v =
    add_namespaced_tags_to_variants<
        std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::internal

#endif
