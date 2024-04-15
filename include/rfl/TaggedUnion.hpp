#ifndef RFL_TAGGEDUNION_HPP_
#define RFL_TAGGEDUNION_HPP_

#include <variant>

#include "define_literal.hpp"
#include "internal/Getter.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/tag_t.hpp"

namespace rfl {

// https://serde.rs/enum-representations.html
template <internal::StringLiteral _discriminator, class... Ts>
struct TaggedUnion {
  static constexpr internal::StringLiteral discrimininator_ = _discriminator;

  /// The type of the underlying variant.
  using VariantType = std::variant<Ts...>;

  /// A literal containing all the tags that are possible
  using PossibleTags = define_literal_t<internal::tag_t<_discriminator, Ts>...>;

  TaggedUnion(const VariantType& _variant) : variant_(_variant) {}

  TaggedUnion(VariantType&& _variant) noexcept
      : variant_(std::move(_variant)) {}

  TaggedUnion(const TaggedUnion<_discriminator, Ts...>& _tagged_union) =
      default;

  TaggedUnion(TaggedUnion<_discriminator, Ts...>&& _tagged_union) noexcept =
      default;

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
  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  TaggedUnion<_discriminator, Ts...>& operator=(T&& _variant) {
    variant_ = std::forward<T>(_variant);
    return *this;
  }

  /// Assigns the underlying object.
  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
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

  static_assert(!PossibleTags::has_duplicates(),
                "Duplicate tags are not allowed inside tagged unions.");

  /// The underlying variant - a TaggedUnion is a thin wrapper
  /// around a variant that is mainly used for parsing.
  VariantType variant_;
};

namespace internal {

template <StringLiteral _discriminator, class... NamedTupleTypes>
struct Getter<TaggedUnion<_discriminator, NamedTupleTypes...>> {
 public:
  /// Retrieves the indicated value from the tuple.
  template <int _index>
  static inline auto& get(
      TaggedUnion<_discriminator, NamedTupleTypes...>& _tu) {
    return Getter<std::variant<NamedTupleTypes...>>::template get<_index>(
        _tu.variant_);
  }

  /// Gets a field by name.
  template <StringLiteral _field_name>
  static inline auto& get(
      TaggedUnion<_discriminator, NamedTupleTypes...>& _tu) {
    return Getter<std::variant<NamedTupleTypes...>>::template get<_field_name>(
        _tu.variant_);
  }

  /// Gets a field by the field type.
  template <class Field>
  static inline auto& get(
      TaggedUnion<_discriminator, NamedTupleTypes...>& _tu) {
    return Getter<std::variant<NamedTupleTypes...>>::template get<Field>(
        _tu.variant_);
  }

  /// Retrieves the indicated value from the tuple.
  template <int _index>
  static inline const auto& get_const(
      const TaggedUnion<_discriminator, NamedTupleTypes...>& _tu) {
    return Getter<std::variant<NamedTupleTypes...>>::template get_const<_index>(
        _tu.variant_);
  }

  /// Gets a field by name.
  template <StringLiteral _field_name>
  static inline const auto& get_const(
      const TaggedUnion<_discriminator, NamedTupleTypes...>& _tu) {
    return Getter<std::variant<NamedTupleTypes...>>::template get_const<
        _field_name>(_tu.variant_);
  }

  /// Gets a field by the field type.
  template <class Field>
  static inline const auto& get_const(
      const TaggedUnion<_discriminator, NamedTupleTypes...>& _tu) {
    return Getter<std::variant<NamedTupleTypes...>>::template get_const<Field>(
        _tu.variant_);
  }
};

}  // namespace internal

}  // namespace rfl

#endif  // RFL_TAGGEDUNION_HPP_
