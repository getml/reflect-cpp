#ifndef RFL_TUPLE_HPP_
#define RFL_TUPLE_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include "internal/nth_element_t.hpp"
#include "internal/tuple/accumulate_sizes.hpp"

namespace rfl {

template <class... FieldTypes>
class Tuple;

template <>
class Tuple<> {
 public:
  Tuple() {}
};

template <class... Types>
class Tuple {
  static constexpr size_t size_ = sizeof...(Types);

  static constexpr auto accumulated_sizes_ =
      internal::tuple::accumulate_sizes<Types...>();

  static constexpr auto seq_ = std::make_integer_sequence<int, size_>{};

  static constexpr unsigned int num_bytes_ =
      std::get<size_>(accumulated_sizes_);

  using DataType = std::array<unsigned char, num_bytes_>;

  template <int _i>
  struct Index {};

 public:
  Tuple(const Types&... _t) { copy_from_types(_t..., seq_); }

  Tuple(Types&&... _t) noexcept { move_from_types(std::move(_t)..., seq_); }

  Tuple() : Tuple(Types()...) {}

  Tuple(const Tuple<Types...>& _other) { copy_from_other(_other, seq_); }

  Tuple(Tuple<Types...>&& _other) noexcept {
    move_from_other(std::move(_other), seq_);
  }

  ~Tuple() { destroy_if_necessary(seq_); }

  /// Gets an element by index.
  template <int _index>
  auto& get() {
    using Type = internal::nth_element_t<_index, Types...>;
    return *(reinterpret_cast<Type*>(data_.data() +
                                     std::get<_index>(accumulated_sizes_)));
  }

  /// Gets an element by index.
  template <int _index>
  const auto& get() const {
    using Type = internal::nth_element_t<_index, Types...>;
    return *(reinterpret_cast<const Type*>(
        data_.data() + std::get<_index>(accumulated_sizes_)));
  }

  /// Assigns the underlying object.
  Tuple<Types...>& operator=(const Tuple<Types...>& _other) {
    if (this == &_other) {
      return *this;
    }
    auto temp = Tuple<Types...>(_other);
    destroy_if_necessary(seq_);
    move_from_other(std::move(temp), seq_);
    return *this;
  }

  /// Assigns the underlying object.
  Tuple<Types...>& operator=(Tuple<Types...>&& _other) noexcept {
    if (this == &_other) {
      return *this;
    }
    destroy_if_necessary(seq_);
    move_from_other(std::move(_other), seq_);
    return *this;
  }

 private:
  template <int... _is>
  void copy_from_other(const Tuple<Types...>& _other,
                       std::integer_sequence<int, _is...>) {
    const auto copy_one = [this]<int _i>(const auto& _other, Index<_i>) {
      using Type = internal::nth_element_t<_i, Types...>;
      new (reinterpret_cast<Type*>(data_.data() + pos<_i>()))
          Type(_other.template get<_i>());
    };
    (copy_one(_other, Index<_is>{}), ...);
  }

  template <int... _is>
  void copy_from_types(const Types&... _types,
                       std::integer_sequence<int, _is...>) {
    const auto copy_one = [this]<int _i>(const auto& _t, Index<_i>) {
      using Type = internal::nth_element_t<_i, Types...>;
      new (reinterpret_cast<Type*>(data_.data() + pos<_i>())) Type(_t);
    };
    (copy_one(_types, Index<_is>{}), ...);
  }

  template <int... _is>
  void destroy_if_necessary(std::integer_sequence<int, _is...>) {
    const auto destroy_one = [](auto& _t) {
      using Type = std::remove_cvref_t<decltype(_t)>;
      if constexpr (std::is_destructible_v<Type>) {
        _t.~Type();
      }
    };
    (destroy_one(get<_is>()), ...);
  }

  template <int... _is>
  void move_from_other(Tuple<Types...>&& _other,
                       std::integer_sequence<int, _is...>) {
    const auto move_one = [this]<int _i>(auto&& _other, Index<_i>) {
      using Type = internal::nth_element_t<_i, Types...>;
      new (reinterpret_cast<Type*>(data_.data() + pos<_i>()))
          Type(std::move(_other.template get<_i>()));
    };
    (move_one(_other, Index<_is>{}), ...);
  }

  template <int... _is>
  void move_from_types(Types&&... _types, std::integer_sequence<int, _is...>) {
    const auto move_one = [this]<int _i>(auto&& _t, Index<_i>) {
      using Type = internal::nth_element_t<_i, Types...>;
      new (reinterpret_cast<Type*>(data_.data() + pos<_i>()))
          Type(std::move(_t));
    };
    (move_one(std::move(_types), Index<_is>{}), ...);
  }

  template <int _i>
  static consteval unsigned int pos() {
    return std::get<_i>(accumulated_sizes_);
  }

 private:
  /// The underlying data, can be any of the underlying types.
  alignas(Types...) DataType data_;
};

/// Gets an element by index.
template <int _index, class... Types>
auto& get(rfl::Tuple<Types...>& _tup) {
  return _tup.template get<_index>();
}

/// Gets an element by index.
template <int _index, class... Types>
const auto& get(const rfl::Tuple<Types...>& _tup) {
  return _tup.template get<_index>();
}

/// Gets an element by index.
template <int _index, class... Types>
auto& get(std::tuple<Types...>& _tup) {
  return std::get<_index>(_tup);
}

/// Gets an element by index.
template <int _index, class... Types>
const auto& get(const std::tuple<Types...>& _tup) {
  return std::get<_index>(_tup);
}

template <class... Types>
auto make_tuple(Types&&... _args) {
  return rfl::Tuple<std::decay_t<Types>...>(std::forward<Types>(_args)...);
}

template <class... Types>
auto tie(Types&... _args) {
  return rfl::Tuple<Types&...>(_args...);
}

template <int N, class T>
struct tuple_element;

template <int N, class... Ts>
struct tuple_element<N, rfl::Tuple<Ts...>> {
  using type = internal::nth_element_t<N, Ts...>;
};

template <int N, class... Ts>
struct tuple_element<N, std::tuple<Ts...>> {
  using type = internal::nth_element_t<N, Ts...>;
};

template <int N, class T>
using tuple_element_t =
    typename rfl::tuple_element<N, std::remove_cvref_t<T>>::type;

template <class T>
struct tuple_size;

template <class... Ts>
struct tuple_size<rfl::Tuple<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};

template <class... Ts>
struct tuple_size<std::tuple<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};

template <class T>
inline constexpr auto tuple_size_v =
    rfl::tuple_size<std::remove_cvref_t<T>>::value;

}  // namespace rfl

namespace std {

/// Gets an element by index.
template <int _index, class... Types>
auto& get(rfl::Tuple<Types...>& _tup) {
  return _tup.template get<_index>();
}

/// Gets an element by index.
template <int _index, class... Types>
const auto& get(const rfl::Tuple<Types...>& _tup) {
  return _tup.template get<_index>();
}

}  // namespace std

#endif
