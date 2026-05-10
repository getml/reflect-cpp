#ifndef RFL_FIELD_HPP_
#define RFL_FIELD_HPP_

#include <string_view>
#include <type_traits>
#include <utility>

#include "Literal.hpp"
#include "default.hpp"
#include "internal/Array.hpp"
#include "internal/StringLiteral.hpp"
#include "internal/wrap_in_rfl_array_t.hpp"

namespace rfl {

/// Used to define a field in the NamedTuple with a compile-time name.
/// A Field associates a compile-time string literal name with a value of type T.
/// This is the building block for creating named tuples and structured data with reflection.
/// @tparam _name The compile-time string literal name of the field
/// @tparam T The type of the value stored in the field
template <internal::StringLiteral _name, class T>
struct Field {
  /// The underlying type (wrapped in array if necessary).
  using Type = internal::wrap_in_rfl_array_t<T>;

  /// The name of the field as a Literal type.
  using Name = rfl::Literal<_name>;

  /// Constructs a Field from a const reference to the value.
  /// @param _value The value to store in the field
  Field(const Type& _value) : value_(_value) {}

  /// Constructs a Field from an rvalue reference to the value.
  /// @param _value The value to store in the field (will be moved)
  Field(Type&& _value) noexcept : value_(std::move(_value)) {}

  /// Move constructor.
  /// @param _field The field to move from
  Field(Field&& _field) noexcept = default;

  /// Copy constructor.
  /// @param _field The field to copy from
  Field(const Field& _field) = default;

  /// Copy constructor from a Field with the same name but different type.
  /// @tparam U The type of the other field
  /// @param _field The field to copy from
  template <class U>
  Field(const Field<_name, U>& _field) : value_(_field.get()) {}

  /// Move constructor from a Field with the same name but different type.
  /// @tparam U The type of the other field
  /// @param _field The field to move from
  template <class U>
  Field(Field<_name, U>&& _field) : value_(std::move(_field.get())) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to the field's type
  /// @param _value The value to convert and store
  template <class U>
    requires std::is_convertible_v<U, Type>
  Field(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to the field's type
  /// @param _value The value to convert and store (will be moved)
  template <class U>
    requires std::is_convertible_v<U, Type>
  Field(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Constructs from a Field with compatible type.
  /// @tparam U Type convertible to the field's type
  /// @param _field The field to copy the value from
  template <class U>
    requires std::is_convertible_v<U, Type>
  Field(const Field<_name, U>& _field) : value_(_field.value()) {}

  /// Assigns the underlying object to its default value.
  /// This constructor allows using rfl::default_value to initialize fields.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  Field(const Default&) : value_(Type()) {}

  /// Destructor.
  ~Field() = default;

  /// The name of the field as a compile-time string literal, for internal use.
  constexpr static const internal::StringLiteral name_ = _name;

  /// Returns the underlying value.
  /// @return Const reference to the stored value
  const Type& get() const noexcept { return value_; }

  /// Returns the underlying value.
  /// @return Reference to the stored value
  Type& get() noexcept { return value_; }

  /// Dereference operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator*() noexcept { return value_; }

  /// Dereference operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator*() const noexcept { return value_; }

  /// Returns the name of the field as a string_view.
  /// @return The field's name
  constexpr static std::string_view name() { return name_.string_view(); }

  /// Function call operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator()() noexcept { return value_; }

  /// Function call operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator()() const noexcept { return value_; }

  /// Assigns a new value to the field.
  /// @param _value The value to assign
  /// @return Reference to this field
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns a new value to the field (move version).
  /// @param _value The value to assign (will be moved)
  /// @return Reference to this field
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns from any type convertible to the field's type.
  /// @tparam U Type convertible to the field's type
  /// @param _value The value to convert and assign
  /// @return Reference to this field
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the field to its default value using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  /// @return Reference to this field
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Copy assignment operator.
  /// @param _field The field to copy from
  /// @return Reference to this field
  Field& operator=(const Field& _field) = default;

  /// Move assignment operator.
  /// @param _field The field to move from
  /// @return Reference to this field
  Field& operator=(Field&& _field) = default;

  /// Assigns from another Field with the same name but different type.
  /// @tparam U The type of the other field
  /// @param _field The field to copy the value from
  /// @return Reference to this field
  template <class U>
  auto& operator=(const Field<_name, U>& _field) {
    value_ = _field.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  auto& operator=(Field<_name, U>&& _field) {
    value_ = std::forward<T>(_field.value_);
    return *this;
  }

  /// Assigns the underlying object.
  void set(const Type& _value) { value_ = _value; }

  /// Assigns the underlying object.
  void set(Type&& _value) { value_ = std::move(_value); }

  /// Returns the underlying object.
  Type& value() noexcept { return value_; }

  /// Returns the underlying object.
  const Type& value() const noexcept { return value_; }

  /// The underlying value.
  Type value_;
};

template <internal::StringLiteral _name, class T>
inline auto make_field(T&& _value) {
  using T0 = std::remove_cvref_t<T>;
  if constexpr (std::is_array_v<T0>) {
    return Field<_name, T0>(internal::Array<T0>(std::forward<T>(_value)));
  } else {
    return Field<_name, T0>(std::forward<T>(_value));
  }
}

}  // namespace rfl

#endif  // RFL_FIELD_HPP_
