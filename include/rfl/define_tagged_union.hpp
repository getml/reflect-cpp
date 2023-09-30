#ifndef RFL_DEFINETAGGEDUNION_HPP_
#define RFL_DEFINETAGGEDUNION_HPP_

#include "rfl/TaggedUnion.hpp"
#include "rfl/internal/StringLiteral.hpp"
#include "rfl/internal/define_tagged_union.hpp"

namespace rfl {

template <internal::StringLiteral _discriminator, class... TaggedUnionTypes>
using define_tagged_union_t =
    typename internal::define_tagged_union<_discriminator,
                                           TaggedUnionTypes...>::type;

}  // namespace rfl

#endif
