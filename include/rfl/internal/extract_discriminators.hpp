#ifndef RFL_INTERNAL_EXTRACTDISTRIMINATORS_HPP_
#define RFL_INTERNAL_EXTRACTDISTRIMINATORS_HPP_

#include <type_traits>

#include "../TaggedUnion.hpp"
#include "../define_literal.hpp"
#include "../field_type.hpp"

namespace rfl {
namespace internal {

template <class TaggedUnionType>
struct extract_discriminators;

template <StringLiteral _discriminator, class... NamedTupleType>
struct extract_discriminators<TaggedUnion<_discriminator, NamedTupleType...>> {
  using type = define_literal_t<
      std::remove_cvref_t<field_type_t<_discriminator, NamedTupleType>>...>;
};

}  // namespace internal
}  // namespace rfl

#endif
