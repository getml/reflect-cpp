#ifndef RFL_HEX_HPP_
#define RFL_HEX_HPP_

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace rfl {

/// Wraps an integral value for hexadecimal string serialization.
/// When serialized, Hex converts the integer to its hexadecimal string representation (e.g., "1a2b").
/// When deserialized, it parses a hex string back to an integer value.
/// This is useful for representing values that are conventionally displayed in hex format,
/// such as memory addresses, color codes, or hardware identifiers.
/// @tparam T The integral type to wrap (must satisfy std::is_integral_v)
template <class T>
  requires std::is_integral_v<T>
struct Hex {
  /// The underlying integral type.
  using Type = T;

  using ReflectionType = std::string;

  /// Default constructor - initializes to zero.
  Hex() : value_(0) {}

  /// Constructs from the underlying integer value.
  /// @param _value The integer value to wrap
  Hex(const Type& _value) : value_(_value) {}

  /// Move constructor.
  /// @param _other The Hex to move from
  Hex(Hex<T>&& _other) noexcept = default;

  /// Copy constructor.
  /// @param _other The Hex to copy from
  Hex(const Hex<T>& _other) = default;

  /// Copy constructor from Hex with compatible type.
  /// @tparam U Type compatible with T
  /// @param _other The Hex to copy the value from
  template <class U>
  Hex(const Hex<U>& _other) : value_(_other.get()) {}

  /// Move constructor from Hex with compatible type.
  /// @tparam U Type compatible with T
  /// @param _other The Hex to move the value from
  template <class U>
  Hex(Hex<U>&& _other) : value_(_other.get()) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store
  template <class U>
    requires std::is_convertible_v<U, Type>
  Hex(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store (will be moved)
  template <class U>
    requires std::is_convertible_v<U, Type>
  Hex(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Constructs from a Hex with compatible type.
  /// @tparam U Type convertible to T
  /// @param _other The Hex to copy the value from
  template <class U>
    requires std::is_convertible_v<U, Type>
  Hex(const Hex<U>& _other) : value_(_other.value()) {}

  /// Constructs from a hexadecimal string representation.
  /// Parses strings like "1a2b" or "0x1A2B" into the corresponding integer value.
  /// @param _str The hexadecimal string to parse (e.g., "ff" or "0xFF")
  Hex(const std::string& _str) {
    std::istringstream(_str) >> std::hex >> value_;
  }

  /// Destructor.
  ~Hex() = default;

  /// Returns the underlying integer value.
  /// @return Const reference to the stored value
  const Type& get() const noexcept { return value_; }

  /// Returns the underlying integer value.
  /// @return Reference to the stored value
  Type& get() noexcept { return value_; }

  /// Dereference operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator*() noexcept { return value_; }

  /// Dereference operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator*() const noexcept { return value_; }

  /// Function call operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator()() noexcept { return value_; }

  /// Function call operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator()() const noexcept { return value_; }

  /// Assigns a new integer value.
  /// @param _value The value to assign
  /// @return Reference to this Hex
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and assign
  /// @return Reference to this Hex
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Copy assignment operator.
  /// @param _other The Hex to copy from
  /// @return Reference to this Hex
  Hex<T>& operator=(const Hex<T>& _other) = default;

  /// Move assignment operator.
  /// @param _other The Hex to move from
  /// @return Reference to this Hex
  Hex<T>& operator=(Hex<T>&& _other) = default;

  /// Assigns from another Hex with compatible type (copy).
  /// @tparam U Type compatible with T
  /// @param _other The Hex to copy the value from
  /// @return Reference to this Hex
  template <class U>
  auto& operator=(const Hex<U>& _other) {
    value_ = _other.get();
    return *this;
  }

  /// Assigns from a hexadecimal string representation.
  /// Parses strings like "1a2b" or "0x1A2B" into the corresponding integer value.
  /// @tparam U Template parameter for compatibility
  /// @param _str The hexadecimal string to parse
  /// @return Reference to this Hex
  template <class U>
  auto& operator=(const std::string& _str) {
    std::istringstream(_str) >> std::hex >> value_;
    return *this;
  }

  /// Assigns from another Hex with compatible type (move).
  /// @tparam U Type compatible with T
  /// @param _other The Hex to move the value from
  /// @return Reference to this Hex
  template <class U>
  auto& operator=(Hex<U>&& _other) {
    value_ = std::move(_other.value_);
    return *this;
  }

  /// Returns the value as a hexadecimal string for serialization.
  /// Converts the integer to hex string representation (e.g., "ff").
  /// @return The hexadecimal string representation
  std::string reflection() const {
    std::stringstream stream;
    stream << std::hex << value_;
    return stream.str();
  }

  /// Sets the integer value.
  /// @param _value The value to set
  void set(const Type& _value) { value_ = _value; }

  /// Returns the value as a hexadecimal string, alias for .reflection().
  /// @return The hexadecimal string representation
  std::string str() const { return reflection(); }

  /// Returns the underlying integer value.
  /// @return Reference to the stored value
  Type& value() noexcept { return value_; }

  /// Returns the underlying integer value (const).
  /// @return Const reference to the stored value
  const Type& value() const noexcept { return value_; }

  /// The underlying integer value.
  Type value_;
};

}  // namespace rfl

#endif  // RFL_FIELD_HPP_
