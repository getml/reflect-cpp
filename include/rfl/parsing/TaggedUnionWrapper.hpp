#ifndef RFL_PARSING_TAGGEDUNIONWRAPPER_HPP_
#define RFL_PARSING_TAGGEDUNIONWRAPPER_HPP_

#include <string>
#include <type_traits>

#include "../Flatten.hpp"
#include "../Rename.hpp"
#include "../internal/StringLiteral.hpp"

namespace rfl {
namespace parsing {

template <class T, class TagType, internal::StringLiteral _discriminator,
          bool _as_const_pointer = true>
struct TaggedUnionWrapperNoFields {
  using Type = std::remove_cvref_t<T>;
  using FlattenedType =
      std::conditional_t<_as_const_pointer, const Type*, Type>;
  rfl::Rename<_discriminator, TagType> tag;
  rfl::Flatten<FlattenedType> fields;
};

template <class T, class TagType, internal::StringLiteral _discriminator,
          bool _as_const_pointer = true>
struct TaggedUnionWrapperWithFields {
  using Type = T;
  using FlattenedType =
      std::conditional_t<_as_const_pointer, const Type*, Type>;
  rfl::Field<_discriminator, TagType> tag;
  rfl::Flatten<FlattenedType> fields;
};

}  // namespace parsing
}  // namespace rfl

#endif
