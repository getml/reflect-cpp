#ifndef RFL_FLATTEN_HPP_
#define RFL_FLATTEN_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rfl {

/// Used to embed another struct into the generated output.
/// When serializing, the fields of the embedded struct are flattened into the parent.
template <class T>
struct Flatten {
  /// The underlying type.
  using Type = std::remove_cvref_t<T>;

  /// Default constructor.
  Flatten() = default;

  /// Constructs a Flatten from a const reference to the underlying type.
  /// @param _value The value to wrap
  Flatten(const Type& _value) : value_(_value) {}

  /// Constructs a Flatten from an rvalue reference to the underlying type.
  /// @param _value The value to wrap (will be moved)
  Flatten(Type&& _value) noexcept : value_(std::forward<Type>(_value)) {}

  /// Copy constructor.
  /// @param _f The Flatten to copy from
  Flatten(const Flatten& _f) = default;

  /// Move constructor.
  /// @param _f The Flatten to move from
  Flatten(Flatten&& _f) noexcept = default;

  /// Constructs from another Flatten with a different but compatible type.
  /// @tparam U The type of the other Flatten
  /// @param _f The Flatten to copy from
  template <class U>
  Flatten(const Flatten<U>& _f) : value_(_f.get()) {}

  /// Constructs from another Flatten with a different but compatible type (move version).
  /// @tparam U The type of the other Flatten
  /// @param _f The Flatten to move from
  template <class U>
  Flatten(Flatten<U>&& _f) : value_(std::move(_f.get())) {}

  /// Constructs from any type convertible to the underlying type.
  /// @tparam U The type to convert from
  /// @param _value The value to convert and wrap
  template <class U>
    requires std::is_convertible_v<U, Type>
  Flatten(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to the underlying type (move version).
  /// @tparam U The type to convert from
  /// @param _value The value to convert and wrap (will be moved)
  template <class U>
    requires std::is_convertible_v<U, Type>
  Flatten(U&& _value) : value_(std::forward<U>(_value)) {}

  /// Destructor.
  ~Flatten() = default;

  /// Returns the underlying object.
  Type& get() noexcept { return value_; }

  /// Returns the underlying object.
  const Type& get() const noexcept { return value_; }

  /// Returns the underlying object.
  Type& operator*() noexcept { return value_; }

  /// Returns the underlying object.
  const Type& operator*() const noexcept { return value_; }

  /// Returns the underlying object.
  Type& operator()() noexcept { return value_; }

  /// Returns the underlying object.
  const Type& operator()() const noexcept { return value_; }

  /// Returns the underlying object.
  Type& value() noexcept { return value_; }

  /// Returns the underlying object.
  const Type& value() const noexcept { return value_; }

  /// Assigns the underlying object.
  /// @param _value The value to assign
  /// @return Reference to this object
  Flatten& operator=(const T& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object (move version).
  /// @param _value The value to assign (will be moved)
  /// @return Reference to this object
  Flatten& operator=(T&& _value) {
    value_ = std::forward<Type>(_value);
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U The type to convert from
  /// @param _value The value to assign
  /// @return Reference to this object
  template <class U>
    requires std::is_convertible_v<U, Type>
  Flatten& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Copy assignment operator.
  /// @param _f The Flatten to copy from
  /// @return Reference to this object
  Flatten& operator=(const Flatten& _f) = default;

  /// Move assignment operator.
  /// @param _f The Flatten to move from
  /// @return Reference to this object
  Flatten& operator=(Flatten&& _f) = default;

  /// Assigns from another Flatten with a different but compatible type.
  /// @tparam U The type of the other Flatten
  /// @param _f The Flatten to copy from
  /// @return Reference to this object
  template <class U>
  Flatten& operator=(const Flatten<U>& _f) {
    value_ = _f.get();
    return *this;
  }

  /// Assigns from another Flatten with a different but compatible type (move version).
  /// @tparam U The type of the other Flatten
  /// @param _f The Flatten to move from
  /// @return Reference to this object
  template <class U>
  Flatten& operator=(Flatten<U>&& _f) {
    value_ = std::move(_f.get());
    return *this;
  }

  /// Three-way comparison operator.
  /// @tparam U The type of the other Flatten
  /// @param _f The Flatten to compare with
  /// @return The result of comparing the underlying values
  template <class U>
  auto operator<=>(const Flatten<U>& _f) const {
    return value_ <=> _f.value_;
  }

  /// Equality comparison operator.
  /// @tparam U The type of the other Flatten
  /// @param _f The Flatten to compare with
  /// @return true if the underlying values are equal
  template <class U>
  bool operator==(const Flatten<U>& _f) const {
    return value_ == _f.get();
  }

  /// Assigns the underlying object.
  /// @param _value The value to assign
  void set(const Type& _value) { value_ = _value; }

  /// Assigns the underlying object (move version).
  /// @param _value The value to assign (will be moved)
  void set(Type&& _value) { value_ = std::forward<Type>(_value); }

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
