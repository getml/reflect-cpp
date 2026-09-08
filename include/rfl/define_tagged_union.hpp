#ifndef RFL_DEFINETAGGEDUNION_HPP_
#define RFL_DEFINETAGGEDUNION_HPP_

#include "TaggedUnion.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/define_tagged_union.hpp"

namespace rfl {

/// Defines a tagged union type by combining multiple variant types into a
/// single std::variant. Each alternative from the provided variant types is
/// included in the resulting union variant. For example, combining
/// std::variant<int, double> and std::variant<std::string, bool> produces
/// std::variant<int, double, std::string, bool>.
/// @tparam _discriminator A string literal used to distinguish this tagged
/// union type
/// @tparam TaggedUnionTypes The variant types to merge into the tagged union
template <internal::StringLiteral _discriminator, class... TaggedUnionTypes>
using define_tagged_union_t =
    typename internal::define_tagged_union<_discriminator,
                                           TaggedUnionTypes...>::type;

}  // namespace rfl

#endif
