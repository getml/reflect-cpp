#ifndef RFL_TAGGEDUNION_HPP_
#define RFL_TAGGEDUNION_HPP_

#include <variant>

#include "rfl/internal/StringLiteral.hpp"

namespace rfl {

// https://serde.rs/enum-representations.html
template <internal::StringLiteral _discriminator, class... NamedTupleTypes>
struct TaggedUnion {
    static constexpr internal::StringLiteral discrimininator_ = _discriminator;

    /// The type of the underlying variant.
    using VariantType = std::variant<NamedTupleTypes...>;

    /// Returns the underlying variant.
    inline VariantType& variant() { return variant_; }

    /// Returns the underlying variant.
    inline const VariantType& variant() const { return variant_; }

    /// The underlying variant - a TaggedUnion is a thin wrapper
    /// around a variant that is mainly used for parsing.
    VariantType variant_;
};

}  // namespace rfl

#endif  // RFL_TAGGEDUNION_HPP_
