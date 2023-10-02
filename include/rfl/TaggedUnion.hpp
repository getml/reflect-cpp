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

    TaggedUnion(const VariantType& _variant) : variant_(_variant) {}

    TaggedUnion(VariantType&& _variant)
        : variant_(std::forward<VariantType>(_variant)) {}

    TaggedUnion(const TaggedUnion<_discriminator, VariantType>& _tagged_union)
        : variant_(_tagged_union.variant_) {}

    TaggedUnion(TaggedUnion<_discriminator, VariantType>&& _tagged_union)
        : variant_(std::forward<VariantType>(_tagged_union.variant_)) {}

    template <class T,
              typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                      bool>::type = true>
    TaggedUnion(const T& _t) : variant_(_t) {}

    template <class T,
              typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                      bool>::type = true>
    TaggedUnion(T&& _t) : variant_(std::forward<T>(_t)) {}

    ~TaggedUnion() = default;

    /// Assigns the underlying object.
    inline void operator=(const VariantType& _variant) { variant_ = _variant; }

    /// Assigns the underlying object.
    inline void operator=(VariantType&& _variant) {
        variant_ = std::forward<VariantType>(_variant);
    }

    /// Assigns the underlying object.
    template <class T,
              typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                      bool>::type = true>
    inline void operator=(const T& _variant) {
        variant_ = _variant;
    }

    /// Assigns the underlying object.
    inline void operator=(
        const TaggedUnion<_discriminator, VariantType>& _field) {
        variant_ = _field.get();
    }

    /// Assigns the underlying object.
    inline void operator=(TaggedUnion<_discriminator, VariantType>&& _other) {
        variant_ = std::forward<VariantType>(_other.variant_);
    }

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
