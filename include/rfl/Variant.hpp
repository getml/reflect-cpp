#ifndef RFL_VARIANT_HPP_
#define RFL_VARIANT_HPP_

#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>
#include <utility>

#include "internal/element_index.hpp"
#include "internal/nth_element_t.hpp"
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
  Variant(const Variant<AlternativeTypes...>& _other) {
    copy_from_other(_other);
  }

  Variant(Variant<AlternativeTypes...>&& _other) noexcept {
    move_from_other(_other);
  }

  template <class T, typename std::enable_if<is_alternative_type<T>(),
                                             bool>::type = true>
  Variant(const T& _t) {
    copy_from_type(_t);
  }

  template <class T, typename std::enable_if<is_alternative_type<T>(),
                                             bool>::type = true>
  Variant(T&& _t) noexcept {
    move_from_type(_t);
  }

  ~Variant() { destroy_if_necessary(); }

  /// Assigns the underlying object.
  template <class T, typename std::enable_if<is_alternative_type<T>(),
                                             bool>::type = true>
  Variant<AlternativeTypes...>& operator=(const T& _t) {
    auto temp = Variant<AlternativeTypes...>(_t);
    destroy_if_necessary();
    move_from_other(std::move(temp));
    return *this;
  }

  /// Assigns the underlying object.
  template <class T, typename std::enable_if<is_alternative_type<T>(),
                                             bool>::type = true>
  Variant<AlternativeTypes...>& operator=(T&& _t) noexcept {
    destroy_if_necessary();
    move_from_type(_t);
    return *this;
  }

  /// Assigns the underlying object.
  Variant<AlternativeTypes...>& operator=(
      const Variant<AlternativeTypes...>& _other) {
    if (this == &_other) {
      return *this;
    }
    auto temp = Variant<AlternativeTypes...>(_other);
    destroy_if_necessary();
    move_from_other(std::move(temp));
    return *this;
  }

  /// Assigns the underlying object.
  Variant<AlternativeTypes...>& operator=(
      Variant<AlternativeTypes...>&& _other) noexcept {
    if (this == &_other) {
      return *this;
    }
    destroy_if_necessary();
    move_from_other(_other);
    return *this;
  }

  template <class F>
  auto visit(const F& _f) {
    using FirstAlternative = internal::nth_element_t<0, AlternativeTypes...>;
    using ResultType =
        std::remove_cvref_t<std::result_of_t<F, FirstAlternative>>;
    if constexpr (std::is_same_v<ResultType, void>) {
      do_visit_no_result(_f, std::make_integer_sequence<ValueType, size_>());
    } else {
      auto res = std::optional<ResultType>();
      do_visit_with_result(_f, &res,
                           std::make_integer_sequence<ValueType, size_>());
      return res;
    }
  }

  template <class F>
  auto visit(const F& _f) const {
    using FirstAlternative = internal::nth_element_t<0, AlternativeTypes...>;
    using ResultType =
        std::remove_cvref_t<std::result_of_t<F, FirstAlternative>>;
    auto res = std::optional<ResultType>();
    do_visit_with_result(_f, &res,
                         std::make_integer_sequence<ValueType, size_>());
    return res;
  }

 private:
  void copy_from_other(const Variant<AlternativeTypes...>& _other) {
    const auto copy_one = [this](const auto& _t) { copy_from_type(_t); };
    _other.visit(copy_one);
  }

  template <class T>
  void copy_from_type(const T& _t) noexcept {
    using CurrentType = std::remove_cvref_t<decltype(_t)>;
    value_ =
        internal::element_index<CurrentType,
                                std::remove_cvref_t<AlternativeTypes>...>();
    new (reinterpret_cast<CurrentType*>(data_.data())) CurrentType(_t);
  }

  void destroy_if_necessary() {
    const auto destroy_one = [](auto& _t) {
      using T = std::remove_cvref_t<decltype(_t)>;
      if constexpr (std::is_destructible_v<T>) {
        _t.~T();
      }
    };
    visit(destroy_one);
  }

  template <class F, ValueType... _is>
  void do_visit_no_result(const F& _f,
                          std::integer_sequence<ValueType, _is...>) {
    const auto visit_one = [this]<ValueType _i>(const F& _f) {
      if (value_ == _i) {
        _f(get_alternative<_i>());
      }
    };
    (visit_one<_is>(_f), ...);
  }

  template <class F, class ResultType, ValueType... _is>
  void do_visit_with_result(const F& _f, std::optional<ResultType>* _res,
                            std::integer_sequence<ValueType, _is...>) {
    const auto visit_one = [this]<ValueType _i>(
                               const F& _f, std::optional<ResultType>* _res) {
      if (value_ == _i) {
        *_res = _f(get_alternative<_i>());
      }
    };
    (visit_one<_is>(_f, _res), ...);
  }

  template <class F, class ResultType, ValueType... _is>
  void do_visit_with_result(const F& _f, std::optional<ResultType>* _res,
                            std::integer_sequence<ValueType, _is...>) const {
    const auto visit_one = [this]<ValueType _i>(
                               const F& _f, std::optional<ResultType>* _res) {
      if (value_ == _i) {
        *_res = _f(get_alternative<_i>());
      }
    };
    (visit_one<_is>(_f, _res), ...);
  }

  template <ValueType _i>
  auto& get_alternative() noexcept {
    using CurrentType = internal::nth_element_t<_i, AlternativeTypes...>;
    return *(reinterpret_cast<CurrentType*>(data_.data()));
  }

  template <ValueType _i>
  const auto& get_alternative() const noexcept {
    using CurrentType = internal::nth_element_t<_i, AlternativeTypes...>;
    return *(reinterpret_cast<CurrentType*>(data_.data()));
  }

  template <class T>
  static constexpr bool is_alternative_type() {
    return internal::is_element_index<
               std::remove_cvref_t<T>,
               std::remove_cvref_t<AlternativeTypes>...>() != -1;
  }

  void move_from_other(Variant<AlternativeTypes...>&& _other) noexcept {
    const auto move_one = [this](auto&& _t) { move_from_type(std::move(_t)); };
    _other.visit(move_from_type);
  }

  template <class T>
  void move_from_type(T&& _t) noexcept {
    using CurrentType = std::remove_cvref_t<decltype(_t)>;
    value_ =
        internal::element_index<CurrentType,
                                std::remove_cvref_t<AlternativeTypes>...>();
    new (reinterpret_cast<CurrentType*>(data_.data()))
        CurrentType(std::forward<T>(_t));
  }

 private:
  /// Value indicating which of the alternatives is currently contained in the
  /// variant.
  Value value_;

  /// The underlying data, can be any of the underlying types.
  alignas(num_bytes_) DataType data_;
};

template <class F, class... AlternativeTypes>
auto visit(const F& _f, const Variant<AlternativeTypes...>& _v) {
  return _v.visit(_f);
}

template <class F, class... AlternativeTypes>
auto visit(const F& _f, Variant<AlternativeTypes...>&& _v) {
  return _v.visit(_f);
}

}  // namespace rfl

#endif
