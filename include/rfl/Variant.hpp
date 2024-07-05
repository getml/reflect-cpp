#ifndef RFL_VARIANT_HPP_
#define RFL_VARIANT_HPP_

#include <bits/utility.h>

#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>

#include "internal/variant/find_max_size.hpp"

namespace rfl {

template <class... AlternativeTypes>
class Variant {
  static constexpr num_bytes_ = find_max_size<0, AlternativeTypes...>();

  using ValueType =
      std::conditional_t<sizeof...(AlternativeTypes) <=
                             std::numeric_limits<std::uint8_t>::max(),
                         std::uint8_t, std::uint16_t>;

  using DataType = std::array<unsigned char, num_bytes_>;

  static constexpr ValueType size_ = sizeof...(AlternativeTypes);

 public:
  // Variant(const VariantType& _variant) : variant_(_variant) {}

  // Variant(VariantType&& _variant) noexcept : variant_(std::move(_variant)) {}

  Variant(const Variant<AlternativeTypes...>& _variant) = default;

  Variant(Variant<AlternativeTypes...>&& _variant) noexcept = default;

  /*template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  Variant(const T& _t) : variant_(_t) {}

  template <class T,
            typename std::enable_if<std::is_convertible_v<T, VariantType>,
                                    bool>::type = true>
  Variant(T&& _t) noexcept : variant_(std::forward<T>(_t)) {}*/

  ~Variant() = default;
  /*
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
    }*/

  /// Assigns the underlying object.
  Variant<AlternativeTypes...>& operator=(
      const Variant<AlternativeTypes...>& _other) = default;

  /// Assigns the underlying object.
  Variant<AlternativeTypes...>& operator=(
      Variant<AlternativeTypes...>&& _other) = default;

  template <class F>
  auto visit(const F& _f) const {
    using ResultType = std::result_of_t<F, ...>;
    auto res = std::optional<ResultType>();
    do_visit_with_result(_f, &res,
                         std::make_integer_sequence<ValueType, size_>());
    return res;
  }

 private:
  template <class F, class ResultType, ValueType... _is>
  void do_visit_with_result(const F& _f, std::optional<ResultType>* _res,
                            std::integer_sequence<ValueType, _is...>) const {
    const auto visit_one = [this]<ValueType _i>(
                               const F& _f, std::optional<ResultType>* _res) {
      if (value_ == _i) {
        **_res = _f(get_alternative<_i>());
      }
    };
    (visit_one<_is>(_f, _res), ...);
  }

  template <ValueType _i>
  auto& get_alternative() noexcept {
    using CurrentType = ...;
    return *(reinterpret_cast<CurrentType*>(data_.data()));
  }

  template <ValueType _i>
  const auto& get_alternative() const noexcept {
    using CurrentType = ...;
    return *(reinterpret_cast<CurrentType*>(data_.data()));
  }

 private:
  /// Value indicating which of the alternatives is currently contained in the
  /// variant.
  Value value_;

  /// The underlying data, can be any of the underlying types.
  alignas(num_bytes_) DataType data_;
};

}  // namespace rfl

#endif
