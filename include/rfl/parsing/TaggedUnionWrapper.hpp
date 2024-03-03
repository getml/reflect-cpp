#ifndef RFL_PARSING_TAGGEDUNIONWRAPPER_HPP_
#define RFL_PARSING_TAGGEDUNIONWRAPPER_HPP_

#include <string>

#include "../Flatten.hpp"
#include "../Rename.hpp"
#include "../internal/StringLiteral.hpp"

namespace rfl {
namespace parsing {

template <class T, class TagType, internal::StringLiteral _discriminator>
struct TaggedUnionWrapperNoFields {
  using Type = T;
  rfl::Rename<_discriminator, TagType> tag;
  rfl::Flatten<const T*> fields;
};

template <class T, class TagType, internal::StringLiteral _discriminator>
struct TaggedUnionWrapperWithFields {
  using Type = T;
  rfl::Field<_discriminator, TagType> tag;
  rfl::Flatten<const T*> fields;
};

}  // namespace parsing
}  // namespace rfl

#endif
