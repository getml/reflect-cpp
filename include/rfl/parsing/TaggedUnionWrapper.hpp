#ifndef RFL_PARSING_TAGGEDUNIONWRAPPER_HPP_
#define RFL_PARSING_TAGGEDUNIONWRAPPER_HPP_

#include <concepts>

#include "rfl/Flatten.hpp"
#include "rfl/Rename.hpp"
#include "rfl/internal/StringLiteral.hpp"

namespace rfl {
namespace parsing {

template <class T, class LiteralType, internal::StringLiteral _discriminator>
struct TaggedUnionWrapperNoFields {
  rfl::Rename<_discriminator, LiteralType> tag;
  rfl::Flatten<const T*> fields;
};

template <class T, class LiteralType, internal::StringLiteral _discriminator>
struct TaggedUnionWrapperWithFields {
  rfl::Field<_discriminator, LiteralType> tag;
  rfl::Flatten<const T*> fields;
};

}  // namespace parsing
}  // namespace rfl

#endif
