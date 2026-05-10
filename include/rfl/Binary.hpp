#ifndef RFL_BINARY_HPP_
#define RFL_BINARY_HPP_

#include <algorithm>
#include <bitset>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace rfl {

/// Wraps an unsigned integer value for binary string serialization.
/// When serialized, Binary converts the integer to its binary string
/// representation (e.g., "101010"). When deserialized, it parses a binary
/// string back to an integer value. This is useful for representing flags, bit
/// patterns, or when you need human-readable binary format.
/// @tparam T The unsigned integer type to wrap (must satisfy
/// std::is_unsigned_v)
template <class T>
  requires std::is_unsigned_v<T>
struct Binary {
  /// The underlying unsigned integer type.
  using Type = T;

  using ReflectionType = std::string;

  using Bitset = std::bitset<sizeof(Type) * 8>;

  /// Default constructor - initializes to zero.
  Binary() : value_(0) {}

  /// Constructs from the underlying integer value.
  /// @param _value The integer value to wrap
  Binary(const Type& _value) : value_(_value) {}

  /// Move constructor.
  /// @param _other The Binary to move from
  Binary(Binary&& _other) noexcept = default;

  /// Copy constructor.
  /// @param _other The Binary to copy from
  Binary(const Binary& _other) = default;

  /// Copy constructor from Binary with compatible type.
  /// @tparam U Type compatible with T
  /// @param _other The Binary to copy the value from
  template <class U>
  Binary(const Binary<U>& _other) : value_(_other.get()) {}

  /// Move constructor from Binary with compatible type.
  /// @tparam U Type compatible with T
  /// @param _other The Binary to move the value from
  template <class U>
  Binary(Binary<U>&& _other) : value_(_other.get()) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store
  template <class U>
    requires std::is_convertible_v<U, Type>
  Binary(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store (will be moved)
  template <class U>
    requires std::is_convertible_v<U, Type>
  Binary(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Constructs from a Binary with compatible type.
  /// @tparam U Type convertible to T
  /// @param _other The Binary to copy the value from
  template <class U>
    requires std::is_convertible_v<U, Type>
  Binary(const Binary<U>& _other) : value_(_other.value()) {}

  /// Constructs from a binary string representation.
  /// Parses strings like "101010" into the corresponding integer value.
  /// @param _str The binary string to parse (e.g., "10101010")
  Binary(const std::string& _str)
      : value_(static_cast<T>(Bitset{_str}.to_ullong())) {}

  /// Destructor.
  ~Binary() = default;

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
  /// @return Reference to this Binary
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and assign
  /// @return Reference to this Binary
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Copy assignment operator.
  /// @param _other The Binary to copy from
  /// @return Reference to this Binary
  Binary& operator=(const Binary<T>& _other) = default;

  /// Move assignment operator.
  /// @param _other The Binary to move from
  /// @return Reference to this Binary
  Binary& operator=(Binary<T>&& _other) = default;

  /// Assigns from another Binary with compatible type (copy).
  /// @tparam U Type compatible with T
  /// @param _other The Binary to copy the value from
  /// @return Reference to this Binary
  template <class U>
  auto& operator=(const Binary<U>& _other) {
    value_ = _other.get();
    return *this;
  }

  /// Assigns from a binary string representation.
  /// Parses strings like "101010" into the corresponding integer value.
  /// @tparam U Template parameter for compatibility
  /// @param _str The binary string to parse
  /// @return Reference to this Binary
  auto& operator=(const std::string& _str) {
    value_ = static_cast<T>(Bitset{_str}.to_ullong());
    return *this;
  }

  /// Assigns from another Binary with compatible type (move).
  /// @tparam U Type compatible with T
  /// @param _other The Binary to move the value from
  /// @return Reference to this Binary
  template <class U>
  auto& operator=(Binary<U>&& _other) {
    value_ = std::move(_other.value_);
    return *this;
  }

  /// Returns the value as a binary string for serialization.
  /// Converts the integer to binary string representation (e.g., "10101010").
  /// @return The binary string representation
  std::string reflection() const { return Bitset{value_}.to_string(); }

  /// Sets the integer value.
  /// @param _value The value to set
  void set(const Type& _value) { value_ = _value; }

  /// Returns the value as a binary string, alias for .reflection().
  /// @return The binary string representation
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
