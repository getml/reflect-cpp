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

    TaggedUnion(VariantType&& _variant) noexcept
        : variant_(std::move(_variant)) {}

    TaggedUnion(const TaggedUnion<_discriminator, NamedTupleTypes...>&
                    _tagged_union) = default;

    TaggedUnion(TaggedUnion<_discriminator, NamedTupleTypes...>&&
                    _tagged_union) noexcept = default;

    template <class T,
              typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                      bool>::type = true>
    TaggedUnion(const T& _t) : variant_(_t) {}

    template <class T,
              typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                      bool>::type = true>
    TaggedUnion(T&& _t) noexcept : variant_(std::forward<T>(_t)) {}

    ~TaggedUnion() = default;

    /// Assigns the underlying object.
    TaggedUnion<_discriminator, NamedTupleTypes...>& operator=(
        const VariantType& _variant) {
        variant_ = _variant;
        return *this;
    }

    /// Assigns the underlying object.
    TaggedUnion<_discriminator, NamedTupleTypes...>& operator=(
        VariantType&& _variant) {
        variant_ = std::move(_variant);
        return *this;
    }

    /// Assigns the underlying object.
    template <class T,
              typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                      bool>::type = true>
    TaggedUnion<_discriminator, NamedTupleTypes...>& operator=(T&& _variant) {
        variant_ = std::forward<T>(_variant);
        return *this;
    }

    /// Assigns the underlying object.
    template <class T,
              typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                      bool>::type = true>
    TaggedUnion<_discriminator, NamedTupleTypes...>& operator=(
        const T& _variant) {
        variant_ = _variant;
        return *this;
    }

    /// Assigns the underlying object.
    TaggedUnion<_discriminator, NamedTupleTypes...>& operator=(
        const TaggedUnion<_discriminator, NamedTupleTypes...>& _other) =
        default;

    /// Assigns the underlying object.
    TaggedUnion<_discriminator, NamedTupleTypes...>& operator=(
        TaggedUnion<_discriminator, NamedTupleTypes...>&& _other) = default;

    /// Returns the underlying variant.
    VariantType& variant() { return variant_; }

    /// Returns the underlying variant.
    const VariantType& variant() const { return variant_; }

    /// The underlying variant - a TaggedUnion is a thin wrapper
    /// around a variant that is mainly used for parsing.
    VariantType variant_;
};

}  // namespace rfl

#endif  // RFL_TAGGEDUNION_HPP_
