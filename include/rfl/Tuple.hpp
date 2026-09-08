#ifndef RFL_TUPLE_HPP_
#define RFL_TUPLE_HPP_

#include <array>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "internal/nth_element_t.hpp"
#include "internal/ptr_cast.hpp"
#include "internal/tuple/calculate_positions.hpp"

namespace rfl {

template <class... FieldTypes>
class Tuple;

/// Specialization for empty Tuple.
template <>
class Tuple<> {
 public:
  /// Default constructor for empty Tuple.
  Tuple() {}
};

/// Tuple class template for arbitrary types.
template <class... Types>
class Tuple {
  static constexpr size_t size_ = sizeof...(Types);

  static constexpr auto positions_ =
      internal::tuple::calculate_positions<Types...>();

  static constexpr auto seq_ = std::make_integer_sequence<int, size_>{};

  static constexpr unsigned int num_bytes_ = std::get<size_>(positions_);

  using DataType = std::array<unsigned char, num_bytes_>;

 public:
  /// Constructs a Tuple by copying from the provided arguments.
  /// @param _t The values to copy into the Tuple.
  Tuple(const Types&... _t) { copy_from_types(_t..., seq_); }

  /// Constructs a Tuple by moving from the provided arguments.
  /// @param _t The values to move into the Tuple.
  Tuple(Types&&... _t) noexcept { move_from_types(std::move(_t)..., seq_); }

  /// Default constructor. Initializes each element with its default
  /// constructor.
  Tuple() : Tuple(Types()...) {}

  /// Copy constructor. Copies all elements from another Tuple.
  /// @param _other The Tuple to copy from.
  Tuple(const Tuple& _other) { copy_from_other(_other, seq_); }

  /// Move constructor. Moves all elements from another Tuple.
  /// @param _other The Tuple to move from.
  Tuple(Tuple&& _other) noexcept { move_from_other(std::move(_other), seq_); }

  /// Destructor. Destroys all elements if necessary.
  ~Tuple() { destroy_if_necessary(seq_); }

  /// Gets an element by index (mutable).
  /// @tparam _index The index of the element to access.
  /// @return Reference to the element at the specified index.
  template <int _index>
  constexpr auto& get() {
    using Type = internal::nth_element_t<_index, Types...>;
    return *internal::ptr_cast<Type*>(data_.data() + pos<_index>());
  }

  /// Gets an element by index (const).
  /// @tparam _index The index of the element to access.
  /// @return Const reference to the element at the specified index.
  template <int _index>
  constexpr const auto& get() const {
    using Type = internal::nth_element_t<_index, Types...>;
    return *internal::ptr_cast<const Type*>(data_.data() + pos<_index>());
  }

  /// Copy assignment operator. Assigns from another Tuple by copy.
  /// @param _other The Tuple to copy from.
  /// @return Reference to this Tuple.
  Tuple& operator=(const Tuple& _other) {
    if (this == &_other) {
      return *this;
    }
    auto temp = Tuple(_other);
    destroy_if_necessary(seq_);
    move_from_other(std::move(temp), seq_);
    return *this;
  }

  /// Move assignment operator. Assigns from another Tuple by move.
  /// @param _other The Tuple to move from.
  /// @return Reference to this Tuple.
  Tuple& operator=(Tuple&& _other) noexcept {
    if (this == &_other) {
      return *this;
    }
    destroy_if_necessary(seq_);
    move_from_other(std::move(_other), seq_);
    return *this;
  }

  /// Equality operator. Compares two Tuples for equality.
  /// @tparam OtherTypes The types of the other Tuple.
  /// @param _other The Tuple to compare with.
  /// @return True if all elements are equal, false otherwise.
  template <class... OtherTypes>
  bool operator==(const Tuple<OtherTypes...>& _other) const noexcept {
    static_assert(sizeof...(Types) == sizeof...(OtherTypes),
                  "The size of the two tuples must be the same.");
    const auto is_same = [&]<int _i>(std::integral_constant<int, _i>) -> bool {
      return this->get<_i>() == _other.template get<_i>();
    };
    return [&]<int... _is>(std::integer_sequence<int, _is...>) {
      return (true && ... && is_same(std::integral_constant<int, _is>{}));
    }(std::make_integer_sequence<int, sizeof...(Types)>());
  }

  /// Three-way comparison operator. Compares two Tuples lexicographically.
  /// @tparam OtherTypes The types of the other Tuple.
  /// @param _other The Tuple to compare with.
  /// @return The comparison result according to the common comparison category.
  template <class... OtherTypes>
  auto operator<=>(const Tuple<OtherTypes...>& _other) const noexcept {
    static_assert(sizeof...(Types) == sizeof...(OtherTypes),
                  "The size of the two tuples must be the same.");

    using OrderingType = std::common_comparison_category_t<
        decltype(std::declval<Types>() <=> std::declval<OtherTypes>())...>;

    auto ordering = OrderingType::equivalent;

    const auto compare = [&]<int _i>(std::integral_constant<int, _i>) {
      ordering = static_cast<OrderingType>(this->get<_i>() <=>
                                           _other.template get<_i>());
      return ordering != 0;
    };

    return [&]<int... _is>(std::integer_sequence<int, _is...>) {
      (compare(std::integral_constant<int, _is>{}) || ...);
      return ordering;
    }(std::make_integer_sequence<int, sizeof...(Types)>());
  }

 private:
  /// Copies all elements from another Tuple.
  /// @param _other The Tuple to copy from.
  /// @param seq_ Integer sequence for element indices.
  template <int... _is>
  void copy_from_other(const Tuple& _other,
                       std::integer_sequence<int, _is...>) {
    const auto copy_one = [this]<int _i>(const auto& _other,
                                         std::integral_constant<int, _i>) {
      using Type = internal::nth_element_t<_i, Types...>;
      ::new (static_cast<void*>(data_.data() + pos<_i>()))
          Type(_other.template get<_i>());
    };
    (copy_one(_other, std::integral_constant<int, _is>{}), ...);
  }

  /// Copies all elements from provided arguments.
  /// @param _types The values to copy.
  /// @param seq_ Integer sequence for element indices.
  template <int... _is>
  void copy_from_types(const Types&... _types,
                       std::integer_sequence<int, _is...>) {
    const auto copy_one = [this]<int _i>(const auto& _t,
                                         std::integral_constant<int, _i>) {
      using Type = internal::nth_element_t<_i, Types...>;
      ::new (static_cast<void*>(data_.data() + pos<_i>())) Type(_t);
    };
    (copy_one(_types, std::integral_constant<int, _is>{}), ...);
  }

  /// Destroys all elements if they are destructible.
  /// @param seq_ Integer sequence for element indices.
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

  /// Moves all elements from another Tuple.
  /// @param _other The Tuple to move from.
  /// @param seq_ Integer sequence for element indices.
  template <int... _is>
  void move_from_other(Tuple&& _other, std::integer_sequence<int, _is...>) {
    const auto move_one = [this]<int _i>(auto&& _other,
                                         std::integral_constant<int, _i>) {
      using Type = internal::nth_element_t<_i, Types...>;
      ::new (static_cast<void*>(data_.data() + pos<_i>()))
          Type(std::move(_other.template get<_i>()));
    };
    (move_one(_other, std::integral_constant<int, _is>{}), ...);
  }

  /// Moves all elements from provided arguments.
  /// @param _types The values to move.
  /// @param seq_ Integer sequence for element indices.
  template <int... _is>
  void move_from_types(Types&&... _types, std::integer_sequence<int, _is...>) {
    const auto move_one = [this]<int _i>(auto&& _t,
                                         std::integral_constant<int, _i>) {
      using Type = internal::nth_element_t<_i, Types...>;
      ::new (static_cast<void*>(data_.data() + pos<_i>())) Type(std::move(_t));
    };
    (move_one(std::move(_types), std::integral_constant<int, _is>{}), ...);
  }

  /// Returns the byte offset of the element at index _i.
  /// @tparam _i The index of the element.
  /// @return The byte offset of the element.
  template <int _i>
  static consteval unsigned int pos() {
    return std::get<_i>(positions_);
  }

 private:
  /// The underlying data, can be any of the underlying types.
  alignas(Types...) DataType data_;
};

/// Gets an element by index from an rfl::Tuple (mutable).
/// @tparam _index The index of the element.
/// @tparam Types The types in the Tuple.
/// @param _tup The Tuple to access.
/// @return Reference to the element at the specified index.
template <int _index, class... Types>
constexpr auto& get(rfl::Tuple<Types...>& _tup) {
  return _tup.template get<_index>();
}

/// Gets an element by index from an rfl::Tuple (const).
/// @tparam _index The index of the element.
/// @tparam Types The types in the Tuple.
/// @param _tup The Tuple to access.
/// @return Const reference to the element at the specified index.
template <int _index, class... Types>
constexpr const auto& get(const rfl::Tuple<Types...>& _tup) {
  return _tup.template get<_index>();
}

/// Gets an element by index from a std::tuple (mutable).
/// @tparam _index The index of the element.
/// @tparam Types The types in the tuple.
/// @param _tup The tuple to access.
/// @return Reference to the element at the specified index.
template <int _index, class... Types>
constexpr auto& get(std::tuple<Types...>& _tup) {
  return std::get<_index>(_tup);
}

/// Gets an element by index from a std::tuple (const).
/// @tparam _index The index of the element.
/// @tparam Types The types in the tuple.
/// @param _tup The tuple to access.
/// @return Const reference to the element at the specified index.
template <int _index, class... Types>
constexpr const auto& get(const std::tuple<Types...>& _tup) {
  return std::get<_index>(_tup);
}

/// Creates an rfl::Tuple from the provided arguments, decaying their types.
/// @tparam Types The types of the arguments.
/// @param _args The arguments to store in the Tuple.
/// @return An rfl::Tuple containing the arguments.
template <class... Types>
auto make_tuple(Types&&... _args) {
  return rfl::Tuple<std::decay_t<Types>...>(std::forward<Types>(_args)...);
}

/// Provides the type of the N-th element of a Tuple.
/// @tparam N The index of the element.
/// @tparam T The Tuple type.
template <int N, class T>
struct tuple_element;

/// Specialization for rfl::Tuple.
/// @tparam N The index of the element.
/// @tparam Ts The types in the Tuple.
template <int N, class... Ts>
struct tuple_element<N, rfl::Tuple<Ts...>> {
  using type = internal::nth_element_t<N, Ts...>;
};

/// Specialization for std::tuple.
/// @tparam N The index of the element.
/// @tparam Ts The types in the tuple.
template <int N, class... Ts>
struct tuple_element<N, std::tuple<Ts...>> {
  using type = internal::nth_element_t<N, Ts...>;
};

/// Alias for the type of the N-th element of a Tuple.
/// @tparam N The index of the element.
/// @tparam T The Tuple type.
template <int N, class T>
using tuple_element_t =
    typename rfl::tuple_element<N, std::remove_cvref_t<T>>::type;

/// Provides the size (number of elements) of a Tuple.
/// @tparam T The Tuple type.
template <class T>
struct tuple_size;

/// Specialization for rfl::Tuple.
/// @tparam Ts The types in the Tuple.
template <class... Ts>
struct tuple_size<rfl::Tuple<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};

/// Specialization for std::tuple.
/// @tparam Ts The types in the tuple.
template <class... Ts>
struct tuple_size<std::tuple<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};

/// Alias for the size (number of elements) of a Tuple.
/// @tparam T The Tuple type.
template <class T>
inline constexpr auto tuple_size_v =
    rfl::tuple_size<std::remove_cvref_t<T>>::value;

}  // namespace rfl

namespace std {

/// Gets an element by index from an rfl::Tuple (mutable).
/// @tparam _index The index of the element.
/// @tparam Types The types in the Tuple.
/// @param _tup The Tuple to access.
/// @return Reference to the element at the specified index.
template <int _index, class... Types>
constexpr auto& get(rfl::Tuple<Types...>& _tup) {
  return _tup.template get<_index>();
}

/// Gets an element by index from an rfl::Tuple (const).
/// @tparam _index The index of the element.
/// @tparam Types The types in the Tuple.
/// @param _tup The Tuple to access.
/// @return Const reference to the element at the specified index.
template <int _index, class... Types>
constexpr const auto& get(const rfl::Tuple<Types...>& _tup) {
  return _tup.template get<_index>();
}

}  // namespace std

#endif
