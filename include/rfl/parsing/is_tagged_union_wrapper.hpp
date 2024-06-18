#ifndef RFL_PARSING_ISTAGGEDUNIONWRAPPER_HPP_
#define RFL_PARSING_ISTAGGEDUNIONWRAPPER_HPP_

#include <type_traits>

#include "../internal/StringLiteral.hpp"
#include "TaggedUnionWrapper.hpp"

namespace rfl {
namespace parsing {

template <class T>
class is_tagged_union_wrapper;

template <class T>
class is_tagged_union_wrapper : public std::false_type {};

template <class T, class TagType, internal::StringLiteral _discriminator,
          bool _as_const_pointer>
class is_tagged_union_wrapper<
    TaggedUnionWrapperNoFields<T, TagType, _discriminator, _as_const_pointer>>
    : public std::true_type {};

template <class T, class TagType, internal::StringLiteral _discriminator,
          bool _as_const_pointer>
class is_tagged_union_wrapper<
    TaggedUnionWrapperWithFields<T, TagType, _discriminator, _as_const_pointer>>
    : public std::true_type {};

template <class T>
constexpr bool is_tagged_union_wrapper_v =
    is_tagged_union_wrapper<std::remove_cvref_t<T>>::value;

}  // namespace parsing
}  // namespace rfl

#endif
