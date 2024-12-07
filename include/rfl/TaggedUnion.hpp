#ifndef RFL_TAGGEDUNION_HPP_
#define RFL_TAGGEDUNION_HPP_

#include "Variant.hpp"
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
  using VariantType = rfl::Variant<Ts...>;

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

  /// Applies function _f to all underlying alternatives.
  template <class F>
  auto visit(F&& _f)
      -> decltype(std::declval<VariantType>().visit(std::declval<F&&>())) {
    return variant_.visit(std::forward<F>(_f));
  }

  /// Applies function _f to all underlying alternatives.
  template <class F>
  auto visit(F&& _f) const
      -> decltype(std::declval<VariantType>().visit(std::declval<F&&>())) {
    return variant_.visit(std::forward<F>(_f));
  }

  /// The underlying variant - a TaggedUnion is a thin wrapper
  /// around a variant that is mainly used for parsing.
  VariantType variant_;
};

template <typename T>
concept TaggedUnionBased = requires(T t) {
  []<internal::StringLiteral _discriminator, typename... Args>(
      TaggedUnion<_discriminator, Args...> const&) {}(t);
};

template <class T>
struct PossibleTags;

template <internal::StringLiteral _discriminator, class... Ts>
struct PossibleTags<TaggedUnion<_discriminator, Ts...>> {
  using Type = define_literal_t<internal::tag_t<_discriminator, Ts>...>;
};

template <class T>
using possible_tags_t = typename PossibleTags<T>::Type;

template <internal::StringLiteral _discriminator, class... Ts>
bool operator==(
  const TaggedUnion<_discriminator, Ts...>& lhs,
  const TaggedUnion<_discriminator, Ts...>& rhs
  ) {

  return (lhs.variant().index() == rhs.variant().index()) &&
         lhs.variant().visit(
           [&rhs](const auto& l) {
               return rhs.variant().visit(
                 [&l](const auto& r) -> bool {
                   if constexpr (std::is_same_v<std::decay_t<decltype(l)>, std::decay_t<decltype(r)>>)
                     return l == r;
                   else
                     return false;
               }
             );
           }
         );
}

}  // namespace rfl

#endif  // RFL_TAGGEDUNION_HPP_
