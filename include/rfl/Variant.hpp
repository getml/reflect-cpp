#ifndef RFL_VARIANT_HPP_
#define RFL_VARIANT_HPP_

#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "internal/element_index.hpp"
#include "internal/nth_element_t.hpp"
#include "internal/variant/find_max_size.hpp"
#include "internal/variant/is_alternative_type.hpp"
#include "internal/variant/result_t.hpp"
#include "internal/ptr_cast.hpp"

namespace rfl {

template <class... AlternativeTypes>
class Variant {
  static constexpr auto max_size_wrapper_ =
      internal::variant::find_max_size<AlternativeTypes...>();

  static constexpr unsigned long num_bytes_ = max_size_wrapper_.size_;

  using DataType = std::array<unsigned char, num_bytes_>;

  using IndexType =
      std::conditional_t<sizeof...(AlternativeTypes) <=
                             std::numeric_limits<std::uint8_t>::max(),
                         std::uint8_t, std::uint16_t>;

  static constexpr IndexType size_ = sizeof...(AlternativeTypes);

  template <class F>
  using result_t = internal::variant::result_t<F, AlternativeTypes...>;

  template <IndexType _i>
  using Index = std::integral_constant<IndexType, _i>;

  template <class T>
  struct TypeWrapper {};

 public:
  Variant() : index_(IndexType()), data_(DataType()) {
    using FirstAlternative = internal::nth_element_t<0, AlternativeTypes...>;
    move_from_type(FirstAlternative());
  }

  Variant(const Variant<AlternativeTypes...>& _other)
      : index_(IndexType()), data_(DataType()) {
    copy_from_other(_other);
  }

  Variant(Variant<AlternativeTypes...>&& _other) noexcept
      : index_(IndexType()), data_(DataType()) {
    move_from_other(std::move(_other));
  }

  template <class T,
            typename std::enable_if<internal::variant::is_alternative_type<
                                        T, AlternativeTypes...>(),
                                    bool>::type = true>
  Variant(const T& _t) : index_(IndexType()), data_(DataType()) {
    copy_from_type(_t);
  }

  template <class T,
            typename std::enable_if<internal::variant::is_alternative_type<
                                        T, AlternativeTypes...>(),
                                    bool>::type = true>
  Variant(T&& _t) noexcept : index_(IndexType()), data_(DataType()) {
    move_from_type(std::forward<T>(_t));
  }

  ~Variant() { destroy_if_necessary(); }

  /// Emplaces a new element into the variant.
  template <class T, class... Args>
  constexpr T& emplace(Args&&... _args) {
    auto t = T{std::forward<Args>(_args)...};
    destroy_if_necessary();
    move_from_type(std::move(t));
    return *internal::ptr_cast<T*>(data_.data());
  }

  /// Emplaces a new element into the variant.
  template <int _i, class... Args>
  constexpr auto& emplace(Args&&... _args) {
    using T = internal::nth_element_t<_i, AlternativeTypes...>;
    return emplace<T>(std::move(_args)...);
  }

  /// Returns the index of the element currently held.
  constexpr int index() const noexcept { return index_; }

  /// Assigns the underlying object.
  template <class T,
            typename std::enable_if<internal::variant::is_alternative_type<
                                        T, AlternativeTypes...>(),
                                    bool>::type = true>
  Variant<AlternativeTypes...>& operator=(const T& _t) {
    auto temp = Variant<AlternativeTypes...>(_t);
    destroy_if_necessary();
    move_from_other(std::move(temp));
    return *this;
  }

  /// Assigns the underlying object.
  template <class T,
            typename std::enable_if<internal::variant::is_alternative_type<
                                        T, AlternativeTypes...>(),
                                    bool>::type = true>
  Variant<AlternativeTypes...>& operator=(T&& _t) noexcept {
    destroy_if_necessary();
    move_from_type(std::forward<T>(_t));
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
    move_from_other(std::move(_other));
    return *this;
  }

  /// Swaps the content with the other variant.
  void swap(Variant<AlternativeTypes...>& _other) noexcept {
    if (this == &_other) {
      return;
    }
    auto temp = Variant<AlternativeTypes...>(std::move(*this));
    move_from_other(std::move(_other));
    _other = std::move(temp);
  }

  template <class F>
  result_t<F> visit(F&& _f) {
    using ResultType = result_t<F>;
    if constexpr (std::is_same_v<ResultType, void>) {
      bool visited = false;
      do_visit_no_result(std::forward<F>(_f), &visited,
                         std::make_integer_sequence<IndexType, size_>());
    } else if constexpr (std::is_reference_v<ResultType>) {
      std::remove_reference_t<ResultType>* res = nullptr;
      do_visit_with_reference(std::forward<F>(_f), &res,
                              std::make_integer_sequence<IndexType, size_>());
      return *res;
    } else {
      auto res = std::optional<ResultType>();
      do_visit_with_result(std::forward<F>(_f), &res,
                           std::make_integer_sequence<IndexType, size_>());
      return std::move(*res);
    }
  }

  template <class F>
  result_t<F> visit(F&& _f) const {
    using ResultType = result_t<F>;
    if constexpr (std::is_same_v<ResultType, void>) {
      bool visited = false;
      do_visit_no_result(std::forward<F>(_f), &visited,
                         std::make_integer_sequence<IndexType, size_>());
    } else if constexpr (std::is_reference_v<ResultType>) {
      std::remove_reference_t<ResultType>* res = nullptr;
      do_visit_with_reference(std::forward<F>(_f), &res,
                              std::make_integer_sequence<IndexType, size_>());
      return *res;
    } else {
      auto res = std::optional<ResultType>();
      do_visit_with_result(std::forward<F>(_f), &res,
                           std::make_integer_sequence<IndexType, size_>());
      return std::move(*res);
    }
  }

 private:
  void copy_from_other(const Variant<AlternativeTypes...>& _other) {
    const auto copy_one = [this](const auto& _t) { this->copy_from_type(_t); };
    _other.visit(copy_one);
  }

  template <class T>
  void copy_from_other_type(const T& _t) {
    bool set = false;
    const auto copy_one = [&, this]<class AltType>(const T& _t,
                                                   const TypeWrapper<AltType>) {
      if constexpr (std::is_convertible_v<T, AltType>) {
        if (!set) {
          move_from_type(AltType(_t));
          set = true;
        }
      }
    };
    (copy_one(_t, TypeWrapper<AlternativeTypes>{}), ...);
  }

  template <class T>
  void copy_from_type(const T& _t) noexcept {
    using CurrentType = std::remove_cvref_t<decltype(_t)>;
    index_ =
        internal::element_index<CurrentType,
                                std::remove_cvref_t<AlternativeTypes>...>();
    new (data_.data()) CurrentType(_t);
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

  template <class F, IndexType... _is>
  void do_visit_no_result(F& _f, bool* _visited,
                          std::integer_sequence<IndexType, _is...>) {
    auto visit_one = [this]<IndexType _i>(const F& _f, bool* _visited,
                                          Index<_i>) {
      if (!*_visited && index_ == _i) {
        _f(get_alternative<_i>());
        *_visited = true;
      }
    };
    (visit_one(_f, _visited, Index<_is>{}), ...);
  }

  template <class F, IndexType... _is>
  void do_visit_no_result(F& _f, bool* _visited,
                          std::integer_sequence<IndexType, _is...>) const {
    auto visit_one = [this]<IndexType _i>(const F& _f, bool* _visited,
                                          Index<_i>) {
      if (!*_visited && index_ == _i) {
        _f(get_alternative<_i>());
        *_visited = true;
      }
    };
    (visit_one(_f, _visited, Index<_is>{}), ...);
  }

  template <class F, IndexType... _is>
  void do_visit_no_result(const F& _f, bool* _visited,
                          std::integer_sequence<IndexType, _is...>) {
    const auto visit_one = [this]<IndexType _i>(const F& _f, bool* _visited,
                                                Index<_i>) {
      if (!*_visited && index_ == _i) {
        _f(get_alternative<_i>());
        *_visited = true;
      }
    };
    (visit_one(_f, _visited, Index<_is>{}), ...);
  }

  template <class F, IndexType... _is>
  void do_visit_no_result(const F& _f, bool* _visited,
                          std::integer_sequence<IndexType, _is...>) const {
    const auto visit_one = [this]<IndexType _i>(const F& _f, bool* _visited,
                                                Index<_i>) {
      if (!*_visited && index_ == _i) {
        _f(get_alternative<_i>());
        *_visited = true;
      }
    };
    (visit_one(_f, _visited, Index<_is>{}), ...);
  }

  template <class F, class ResultType, IndexType... _is>
  void do_visit_with_result(F& _f, std::optional<ResultType>* _res,
                            std::integer_sequence<IndexType, _is...>) {
    auto visit_one = [this]<IndexType _i>(const F& _f,
                                          std::optional<ResultType>* _res,
                                          Index<_i>) {
      if (!*_res && index_ == _i) {
        _res->emplace(_f(get_alternative<_i>()));
      }
    };
    (visit_one(_f, _res, Index<_is>{}), ...);
  }

  template <class F, class ResultType, IndexType... _is>
  void do_visit_with_result(F& _f, std::optional<ResultType>* _res,
                            std::integer_sequence<IndexType, _is...>) const {
    auto visit_one = [this]<IndexType _i>(const F& _f,
                                          std::optional<ResultType>* _res,
                                          Index<_i>) {
      if (!*_res && index_ == _i) {
        _res->emplace(_f(get_alternative<_i>()));
      }
    };
    (visit_one(_f, _res, Index<_is>{}), ...);
  }

  template <class F, class ResultType, IndexType... _is>
  void do_visit_with_result(const F& _f, std::optional<ResultType>* _res,
                            std::integer_sequence<IndexType, _is...>) {
    const auto visit_one = [this]<IndexType _i>(const F& _f,
                                                std::optional<ResultType>* _res,
                                                Index<_i>) {
      if (!*_res && index_ == _i) {
        _res->emplace(_f(get_alternative<_i>()));
      }
    };
    (visit_one(_f, _res, Index<_is>{}), ...);
  }

  template <class F, class ResultType, IndexType... _is>
  void do_visit_with_result(const F& _f, std::optional<ResultType>* _res,
                            std::integer_sequence<IndexType, _is...>) const {
    const auto visit_one = [this]<IndexType _i>(const F& _f,
                                                std::optional<ResultType>* _res,
                                                Index<_i>) {
      if (!*_res && index_ == _i) {
        _res->emplace(_f(get_alternative<_i>()));
      }
    };
    (visit_one(_f, _res, Index<_is>{}), ...);
  }

  template <class F, class ResultType, IndexType... _is>
  void do_visit_with_reference(F& _f, ResultType** _res,
                               std::integer_sequence<IndexType, _is...>) {
    const auto visit_one = [this]<IndexType _i>(const F& _f, ResultType** _res,
                                                Index<_i>) {
      if (!*_res && index_ == _i) {
        *_res = &_f(get_alternative<_i>());
      }
    };
    (visit_one(_f, _res, Index<_is>{}), ...);
  }

  template <class F, class ResultType, IndexType... _is>
  void do_visit_with_reference(F& _f, ResultType** _res,
                               std::integer_sequence<IndexType, _is...>) const {
    const auto visit_one = [this]<IndexType _i>(const F& _f, ResultType** _res,
                                                Index<_i>) {
      if (!*_res && index_ == _i) {
        *_res = &_f(get_alternative<_i>());
      }
    };
    (visit_one(_f, _res, Index<_is>{}), ...);
  }

  template <class F, class ResultType, IndexType... _is>
  void do_visit_with_reference(const F& _f, ResultType** _res,
                               std::integer_sequence<IndexType, _is...>) {
    const auto visit_one = [this]<IndexType _i>(const F& _f, ResultType** _res,
                                                Index<_i>) {
      if (!*_res && index_ == _i) {
        *_res = &_f(get_alternative<_i>());
      }
    };
    (visit_one(_f, _res, Index<_is>{}), ...);
  }

  template <class F, class ResultType, IndexType... _is>
  void do_visit_with_reference(const F& _f, ResultType** _res,
                               std::integer_sequence<IndexType, _is...>) const {
    const auto visit_one = [this]<IndexType _i>(const F& _f, ResultType** _res,
                                                Index<_i>) {
      if (!*_res && index_ == _i) {
        *_res = &_f(get_alternative<_i>());
      }
    };
    (visit_one(_f, _res, Index<_is>{}), ...);
  }

  template <IndexType _i>
  auto& get_alternative() noexcept {
    using CurrentType = internal::nth_element_t<_i, AlternativeTypes...>;
    return *internal::ptr_cast<CurrentType*>(data_.data());
  }

  template <IndexType _i>
  const auto& get_alternative() const noexcept {
    using CurrentType = internal::nth_element_t<_i, AlternativeTypes...>;
    return *internal::ptr_cast<const CurrentType*>(data_.data());
  }

  void move_from_other(Variant<AlternativeTypes...>&& _other) noexcept {
    const auto move_one = [this](auto&& _t) {
      this->move_from_type(std::forward<std::remove_cvref_t<decltype(_t)>>(_t));
    };
    std::forward<Variant<AlternativeTypes...>>(_other).visit(move_one);
  }

  template <class T>
  void move_from_type(T&& _t) noexcept {
    using CurrentType = std::remove_cvref_t<decltype(_t)>;
    index_ =
        internal::element_index<CurrentType,
                                std::remove_cvref_t<AlternativeTypes>...>();
    new (data_.data()) CurrentType(std::forward<T>(_t));
  }

 private:
  /// Index indicating which of the alternatives is currently contained in the
  /// variant.
  IndexType index_;

  /// The underlying data, can be any of the underlying types.
  alignas(AlternativeTypes...) DataType data_;
};

template <typename V>
concept VariantBased = requires(std::decay_t<V> v) {
  []<typename... Args>(Variant<Args...> const&) {}(v);
};

template <class T, class... Types>
constexpr T* get_if(Variant<Types...>* _v) noexcept {
  const auto get = [](auto& _v) -> T* {
    using Type = std::remove_cvref_t<decltype(_v)>;
    if constexpr (std::is_same<Type, std::remove_cvref_t<T>>()) {
      return &_v;
    } else {
      return nullptr;
    }
  };
  return _v->visit(get);
}

template <class T, class... Types>
constexpr const T* get_if(const Variant<Types...>* _v) noexcept {
  const auto get = [](const auto& _v) -> const T* {
    using Type = std::remove_cvref_t<decltype(_v)>;
    if constexpr (std::is_same<Type, std::remove_cvref_t<T>>()) {
      return &_v;
    } else {
      return nullptr;
    }
  };
  return _v->visit(get);
}

template <int _i, class... Types>
constexpr auto* get_if(Variant<Types...>* _v) noexcept {
  using T = internal::nth_element_t<_i, Types...>;
  return get_if<T>(_v);
}

template <int _i, class... Types>
constexpr auto* get_if(const Variant<Types...>* _v) noexcept {
  using T = internal::nth_element_t<_i, Types...>;
  return get_if<T>(_v);
}

template <class T, class... Types>
constexpr T& get(Variant<Types...>& _v) {
  auto ptr = get_if<T>(&_v);
  if (!ptr) {
    throw std::runtime_error("Variant does not contain signified type.");
  }
  return *ptr;
}

template <class T, class... Types>
constexpr T&& get(Variant<Types...>&& _v) {
  auto ptr = get_if<T>(&_v);
  if (!ptr) {
    throw std::runtime_error("Variant does not contain signified type.");
  }
  return std::move(*ptr);
}

template <class T, class... Types>
constexpr const T& get(const Variant<Types...>& _v) {
  auto ptr = get_if<T>(&_v);
  if (!ptr) {
    throw std::runtime_error("Variant does not contain signified type.");
  }
  return *ptr;
}

template <int _i, class... Types>
constexpr auto& get(Variant<Types...>& _v) {
  auto ptr = get_if<_i>(&_v);
  if (!ptr) {
    throw std::runtime_error("Variant does not contain signified type.");
  }
  return *ptr;
}

template <int _i, class... Types>
constexpr auto&& get(Variant<Types...>&& _v) {
  auto ptr = get_if<_i>(&_v);
  if (!ptr) {
    throw std::runtime_error("Variant does not contain signified type.");
  }
  return std::move(*ptr);
}

template <int _i, class... Types>
constexpr const auto& get(const Variant<Types...>& _v) {
  auto ptr = get_if<_i>(&_v);
  if (!ptr) {
    throw std::runtime_error("Variant does not contain signified type.");
  }
  return *ptr;
}

template <class T, class... Types>
constexpr bool holds_alternative(const Variant<Types...>& _v) noexcept {
  constexpr auto ix = internal::element_index<std::remove_cvref_t<T>,
                                              std::remove_cvref_t<Types>...>();
  static_assert(ix != -1, "Type not supported.");
  return ix == _v.index();
}

template <int N, class T>
struct variant_alternative;

template <int N, class... Types>
struct variant_alternative<N, Variant<Types...>> {
  using type = internal::nth_element_t<N, Types...>;
};

template <int N, class VariantType>
using variant_alternative_t =
    typename variant_alternative<N, std::remove_cvref_t<VariantType>>::type;

template <class T>
struct variant_size;

template <class... Types>
struct variant_size<Variant<Types...>>
    : std::integral_constant<size_t, sizeof...(Types)> {};

template <class VariantType>
constexpr size_t variant_size_v =
    variant_size<std::remove_cvref_t<VariantType>>();

}  // namespace rfl

namespace std {
template <class... Types>
void swap(rfl::Variant<Types...>& _lhs, rfl::Variant<Types...>& _rhs) noexcept {
  _lhs.swap(_rhs);
};
}  // namespace std

#endif
