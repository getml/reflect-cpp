#ifndef RFL_SHORT_HPP_
#define RFL_SHORT_HPP_

#include <type_traits>
#include <utility>

#include "default.hpp"
#include "internal/StringLiteral.hpp"

namespace rfl {

/// Assigns a short CLI alias (e.g. "-p") to a field.
/// For non-CLI formats (JSON, YAML, etc.), this is transparent.
template <internal::StringLiteral _name, class T>
struct Short {
  /// The underlying type.
  using Type = T;

  /// The short name.
  static constexpr auto short_name_ = _name;

  static_assert(_name.length == 1, "Short name must be exactly one character.");

  /// @brief Default constructor.
  /// @details Constructs the underlying value using its default constructor.
  /// @requires Type must be default constructible.
  Short()
    requires std::is_default_constructible_v<Type>
      : value_(Type()) {}

  /// @brief Constructs the Short wrapper from a const reference to the
  /// underlying value.
  /// @param _value The value to assign to the underlying field.
  Short(const Type& _value) : value_(_value) {}

  /// @brief Constructs the Short wrapper by moving the underlying value.
  /// @param _value The value to move into the underlying field.
  Short(Type&& _value) noexcept : value_(std::move(_value)) {}

  /// @brief Move constructor.
  /// @param _field The Short field to move from.
  Short(Short<_name, T>&& _field) noexcept = default;

  /// @brief Copy constructor.
  /// @param _field The Short field to copy from.
  Short(const Short<_name, T>& _field) = default;

  /// @brief Constructs from another Short with a convertible underlying type.
  /// @tparam U The type of the other Short's underlying value.
  /// @param _field The other Short field to copy from.
  template <class U>
  Short(const Short<_name, U>& _field) : value_(_field.get()) {}

  /// @brief Move constructs from another Short with a convertible underlying
  /// type.
  /// @tparam U The type of the other Short's underlying value.
  /// @param _field The other Short field to move from.
  template <class U>
  Short(Short<_name, U>&& _field) : value_(std::move(_field.value_)) {}

  /// @brief Constructs from a value convertible to the underlying type.
  /// @tparam U The type of the value.
  /// @param _value The value to assign.
  template <class U>
    requires std::is_convertible_v<U, Type>
  Short(const U& _value) : value_(_value) {}

  /// @brief Move constructs from a value convertible to the underlying type.
  /// @tparam U The type of the value.
  /// @param _value The value to move.
  template <class U>
    requires std::is_convertible_v<U, Type>
  Short(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// @brief Constructs the underlying object to its default value using Default
  /// tag.
  /// @tparam U The type to default construct (defaults to Type).
  /// @param unused Default tag.
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Short(const Default&) : value_(Type()) {}

  /// @brief Destructor.
  /// @details Defaulted destructor for Short.
  ~Short() = default;

  /// @brief Returns a const reference to the underlying object.
  /// @return Const reference to the underlying value.
  const Type& get() const noexcept { return value_; }

  /// @brief Returns a reference to the underlying object.
  /// @return Reference to the underlying value.
  Type& get() noexcept { return value_; }

  /// @brief Dereference operator.
  /// @return Reference to the underlying value.
  Type& operator*() noexcept { return value_; }

  /// @brief Dereference operator (const).
  /// @return Const reference to the underlying value.
  const Type& operator*() const noexcept { return value_; }

  /// @brief Function call operator.
  /// @return Reference to the underlying value.
  Type& operator()() noexcept { return value_; }

  /// @brief Function call operator (const).
  /// @return Const reference to the underlying value.
  const Type& operator()() const noexcept { return value_; }

  /// @brief Assigns a new value to the underlying object.
  /// @param _value The value to assign.
  /// @return Reference to this Short.
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// @brief Move-assigns a new value to the underlying object.
  /// @param _value The value to move.
  /// @return Reference to this Short.
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// @brief Assigns a value convertible to the underlying type.
  /// @tparam U The type of the value.
  /// @param _value The value to assign.
  /// @return Reference to this Short.
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// @brief Assigns the underlying object to its default value using Default
  /// tag.
  /// @tparam U The type to default construct (defaults to Type).
  /// @param unused Default tag.
  /// @return Reference to this Short.
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// @brief Copy assignment operator.
  /// @param _field The Short field to copy from.
  /// @return Reference to this Short.
  Short<_name, T>& operator=(const Short<_name, T>& _field) = default;

  /// @brief Move assignment operator.
  /// @param _field The Short field to move from.
  /// @return Reference to this Short.
  Short<_name, T>& operator=(Short<_name, T>&& _field) = default;

  /// @brief Assigns from another Short with a convertible underlying type.
  /// @tparam U The type of the other Short's underlying value.
  /// @param _field The other Short field to copy from.
  /// @return Reference to this Short.
  template <class U>
  auto& operator=(const Short<_name, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// @brief Move-assigns from another Short with a convertible underlying type.
  /// @tparam U The type of the other Short's underlying value.
  /// @param _field The other Short field to move from.
  /// @return Reference to this Short.
  template <class U>
  auto& operator=(Short<_name, U>&& _field) {
    value_ = std::move(_field.value_);
    return *this;
  }

  /// @brief Sets the underlying object to a new value.
  /// @param _value The value to assign.
  void set(const Type& _value) { value_ = _value; }

  /// @brief Sets the underlying object to a new value by moving.
  /// @param _value The value to move.
  void set(Type&& _value) { value_ = std::move(_value); }

  /// @brief Returns a reference to the underlying value.
  /// @return Reference to the underlying value.
  Type& value() noexcept { return value_; }

  /// @brief Returns a const reference to the underlying value.
  /// @return Const reference to the underlying value.
  const Type& value() const noexcept { return value_; }

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
