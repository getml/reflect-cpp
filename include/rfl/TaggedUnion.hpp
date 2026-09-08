#ifndef RFL_TAGGEDUNION_HPP_
#define RFL_TAGGEDUNION_HPP_

#include "Variant.hpp"
#include "define_literal.hpp"
#include "internal/Getter.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/tag_t.hpp"

namespace rfl {

/// A tagged union (also known as discriminated union) that serializes with an explicit discriminator field.
/// The discriminator field is used to identify which variant alternative is active.
/// See: https://serde.rs/enum-representations.html
template <internal::StringLiteral _discriminator, class... Ts>
struct TaggedUnion {
  static constexpr internal::StringLiteral discriminator_ = _discriminator;

  /// The type of the underlying variant.
  using VariantType = rfl::Variant<Ts...>;

  /// Default constructor.
  TaggedUnion() = default;

  /// Constructs from a variant (copy).
  /// @param _variant The variant to wrap
  TaggedUnion(const VariantType& _variant) : variant_(_variant) {}

  /// Constructs from a variant (move).
  /// @param _variant The variant to wrap (will be moved)
  TaggedUnion(VariantType&& _variant) noexcept
      : variant_(std::move(_variant)) {}

  /// Copy constructor.
  /// @param _tagged_union The tagged union to copy from
  TaggedUnion(const TaggedUnion<_discriminator, Ts...>& _tagged_union) =
      default;

  /// Move constructor.
  /// @param _tagged_union The tagged union to move from
  TaggedUnion(TaggedUnion<_discriminator, Ts...>&& _tagged_union) noexcept =
      default;

  /// Constructs from any type convertible to the underlying variant (copy).
  /// @tparam T The type to convert from
  /// @param _t The value to convert and wrap
  template <class T>
    requires std::is_convertible_v<T, VariantType>
  TaggedUnion(const T& _t) : variant_(_t) {}

  /// Constructs from any type convertible to the underlying variant (move).
  /// @tparam T The type to convert from
  /// @param _t The value to convert and wrap (will be moved)
  template <class T>
    requires std::is_convertible_v<T, VariantType>
  TaggedUnion(T&& _t) noexcept : variant_(std::forward<T>(_t)) {}

  /// Destructor.
  ~TaggedUnion() = default;

  /// Assigns the underlying object.
  TaggedUnion<_discriminator, Ts...>& operator=(const VariantType& _variant) {
    variant_ = _variant;
    return *this;
  }

  /// Assigns the underlying object.
  TaggedUnion<_discriminator, Ts...>& operator=(VariantType&& _variant) {
    variant_ = std::move(_variant);
    return *this;
  }

  /// Assigns the underlying object.
  template <class T>
    requires std::is_convertible_v<T, VariantType>
  TaggedUnion<_discriminator, Ts...>& operator=(T&& _variant) {
    variant_ = std::forward<T>(_variant);
    return *this;
  }

  /// Assigns the underlying object.
  template <class T>
    requires std::is_convertible_v<T, VariantType>
  TaggedUnion<_discriminator, Ts...>& operator=(const T& _variant) {
    variant_ = _variant;
    return *this;
  }

  /// Assigns the underlying object.
  TaggedUnion<_discriminator, Ts...>& operator=(
      const TaggedUnion<_discriminator, Ts...>& _other) = default;

  /// Assigns the underlying object.
  TaggedUnion<_discriminator, Ts...>& operator=(
      TaggedUnion<_discriminator, Ts...>&& _other) = default;

  /// Returns the underlying variant.
  VariantType& variant() { return variant_; }

  /// Returns the underlying variant.
  const VariantType& variant() const { return variant_; }

  /// Applies function _f to all underlying alternatives (mutable version).
  /// This is the visitor pattern - the function will be called with the currently active variant alternative.
  /// @tparam F The type of the visitor function
  /// @param _f The function to apply to the active alternative
  /// @return The result of applying _f to the active alternative
  template <class F>
  auto visit(F&& _f)
      -> decltype(std::declval<VariantType>().visit(std::declval<F&&>())) {
    return variant_.visit(std::forward<F>(_f));
  }

  /// Applies function _f to all underlying alternatives (const version).
  /// This is the visitor pattern - the function will be called with the currently active variant alternative.
  /// @tparam F The type of the visitor function
  /// @param _f The function to apply to the active alternative
  /// @return The result of applying _f to the active alternative
  template <class F>
  auto visit(F&& _f) const
      -> decltype(std::declval<VariantType>().visit(std::declval<F&&>())) {
    return variant_.visit(std::forward<F>(_f));
  }

  /// The underlying variant - a TaggedUnion is a thin wrapper
  /// around a variant that is mainly used for parsing.
  VariantType variant_;
};

/// Concept to check if a type is based on TaggedUnion.
template <typename T>
concept TaggedUnionBased = requires(T t) {
  []<internal::StringLiteral _discriminator, typename... Args>(
      TaggedUnion<_discriminator, Args...> const&) {}(t);
};

/// Helper to extract possible tag values from a TaggedUnion.
template <class T>
struct PossibleTags;

/// Specialization for TaggedUnion - extracts all possible discriminator tags.
/// @tparam _discriminator The name of the discriminator field
/// @tparam Ts The types of the alternatives in the tagged union
template <internal::StringLiteral _discriminator, class... Ts>
struct PossibleTags<TaggedUnion<_discriminator, Ts...>> {
  using Type = define_literal_t<internal::tag_t<_discriminator, Ts>...>;
};

/// Type alias for extracting possible tags from a TaggedUnion type.
template <class T>
using possible_tags_t = typename PossibleTags<T>::Type;

/// Equality operator for TaggedUnion.
/// Two tagged unions are equal if they hold the same alternative and the values are equal.
/// @tparam _discriminator The name of the discriminator field
/// @tparam Ts The types of the alternatives in the tagged union
/// @param lhs The left-hand side tagged union
/// @param rhs The right-hand side tagged union
/// @return true if both tagged unions hold the same alternative with equal values
template <internal::StringLiteral _discriminator, class... Ts>
bool operator==(const TaggedUnion<_discriminator, Ts...>& lhs,
                const TaggedUnion<_discriminator, Ts...>& rhs) {
  return (lhs.variant().index() == rhs.variant().index()) &&
         lhs.variant().visit([&rhs](const auto& l) {
           return rhs.variant().visit([&l](const auto& r) -> bool {
             if constexpr (std::is_same_v<std::decay_t<decltype(l)>,
                                          std::decay_t<decltype(r)>>)
               return l == r;
             else
               return false;
           });
         });
}

}  // namespace rfl

#endif  // RFL_TAGGEDUNION_HPP_
