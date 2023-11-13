#ifndef RFL_VARIANT_HPP_
#define RFL_VARIANT_HPP_

#include <variant>

namespace rfl {

template <class... AlternativeTypes>
struct Variant {
  /// The type of the underlying variant.
  using VariantType = std::variant<AlternativeTypes...>;

  Variant(const VariantType& _variant) : variant_(_variant) {}

  Variant(VariantType&& _variant) noexcept : variant_(std::move(_variant)) {}

  Variant(const Variant<AlternativeTypes...>& _variant) = default;

  Variant(Variant<AlternativeTypes...>&& _variant) noexcept = default;

  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  Variant(const T& _t) : variant_(_t) {}

  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  Variant(T&& _t) noexcept : variant_(std::forward<T>(_t)) {}

  ~Variant() = default;

  /// Assigns the underlying object.
  Variant<AlternativeTypes...>& operator=(const VariantType& _variant) {
    variant_ = _variant;
    return *this;
  }

  /// Assigns the underlying object.
  Variant<AlternativeTypes...>& operator=(VariantType&& _variant) {
    variant_ = std::move(_variant);
    return *this;
  }

  /// Assigns the underlying object.
  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  Variant<AlternativeTypes...>& operator=(T&& _variant) {
    variant_ = std::forward<T>(_variant);
    return *this;
  }

  /// Assigns the underlying object.
  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  Variant<AlternativeTypes...>& operator=(const T& _variant) {
    variant_ = _variant;
    return *this;
  }

  /// Assigns the underlying object.
  Variant<AlternativeTypes...>& operator=(
      const Variant<AlternativeTypes...>& _other) = default;

  /// Assigns the underlying object.
  Variant<AlternativeTypes...>& operator=(
      Variant<AlternativeTypes...>&& _other) = default;

  /// Returns the underlying variant.
  VariantType& variant() { return variant_; }

  /// Returns the underlying variant.
  const VariantType& variant() const { return variant_; }

  /// The underlying variant - a Variant is a thin wrapper
  /// around a variant that is mainly used for parsing.
  VariantType variant_;
};

}  // namespace rfl

#endif
