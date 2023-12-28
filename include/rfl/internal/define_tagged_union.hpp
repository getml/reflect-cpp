#ifndef RFL_INTERNAL_DEFINETAGGEDUNION_HPP_
#define RFL_INTERNAL_DEFINETAGGEDUNION_HPP_

#include "../TaggedUnion.hpp"
#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

/// Allows you to combine several tagged unions.
template <StringLiteral _discriminator, class... TaggedUnionTypes>
struct define_tagged_union;

/// Recursive case - both tagged union.
template <StringLiteral _discriminator, class... NamedTupleTypes1,
          class... NamedTupleTypes2, class... Tail>
struct define_tagged_union<
    _discriminator, TaggedUnion<_discriminator, NamedTupleTypes1...>,
    TaggedUnion<_discriminator, NamedTupleTypes2...>, Tail...> {
    using type = typename define_tagged_union<
        _discriminator,
        TaggedUnion<_discriminator, NamedTupleTypes1..., NamedTupleTypes2...>,
        Tail...>::type;
};

/// Recursive case - tagged union plus named tuple.
template <StringLiteral _discriminator, class... NamedTupleTypes,
          class... FieldTypes, class... Tail>
struct define_tagged_union<_discriminator,
                           TaggedUnion<_discriminator, NamedTupleTypes...>,
                           NamedTuple<FieldTypes...>, Tail...> {
    using type = typename define_tagged_union<
        _discriminator,
        TaggedUnion<_discriminator, NamedTupleTypes...,
                    NamedTuple<FieldTypes...>>,
        Tail...>::type;
};

/// Recursive case - named tuple.
template <StringLiteral _discriminator, class... FieldTypes, class... Tail>
struct define_tagged_union<_discriminator, NamedTuple<FieldTypes...>, Tail...> {
    using type = typename define_tagged_union<
        _discriminator, TaggedUnion<_discriminator, NamedTuple<FieldTypes...>>,
        Tail...>::type;
};

/// Special case - only a single TaggedUnion is left.
template <StringLiteral _discriminator, class... NamedTupleTypes>
struct define_tagged_union<_discriminator,
                           TaggedUnion<_discriminator, NamedTupleTypes...>> {
    using type = TaggedUnion<_discriminator, NamedTupleTypes...>;
};

}  // namespace internal
}  // namespace rfl

#endif
