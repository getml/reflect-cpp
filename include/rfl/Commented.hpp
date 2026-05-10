#ifndef RFL_COMMENTED_HPP_
#define RFL_COMMENTED_HPP_

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "default.hpp"

namespace rfl {

/// Wraps a field value with an optional comment for formats that support
/// comments. The comment is preserved during serialization for formats like
/// TOML and YAML that support inline comments. For formats without comment
/// support, the comment is ignored. This allows you to maintain
/// documentation/metadata alongside your data.
/// @tparam T The type of the field value
template <class T>
struct Commented {
 public:
  using Type = std::remove_cvref_t<T>;
  using ReflectionType = Type;

  /// Default constructor - initializes with default-constructed value and no
  /// comment.
  Commented() : value_(Type()) {}

  /// Constructs from a value without a comment.
  /// @param _value The value to store
  Commented(const Type& _value) : value_(_value) {}

  /// Constructs from a value without a comment (move version).
  /// @param _value The value to store (will be moved)
  Commented(Type&& _value) noexcept : value_(std::move(_value)) {}

  /// Constructs from a value with an optional comment.
  /// @param _value The value to store
  /// @param _comment The optional comment to associate with the value
  Commented(const Type& _value, std::optional<std::string> _comment)
      : comment_(std::move(_comment)), value_(_value) {}

  /// Constructs from a value with an optional comment (move version).
  /// @param _value The value to store (will be moved)
  /// @param _comment The optional comment to associate with the value
  Commented(Type&& _value, std::optional<std::string> _comment) noexcept
      : comment_(std::move(_comment)), value_(std::move(_value)) {}

  /// Move constructor.
  /// @param _commented The Commented to move from
  Commented(Commented&& _commented) noexcept = default;

  /// Copy constructor.
  /// @param _commented The Commented to copy from
  Commented(const Commented& _commented) = default;

  /// Copy constructor from Commented with compatible type.
  /// @tparam U Type compatible with T
  /// @param _commented The Commented to copy from
  template <class U>
  Commented(const Commented<U>& _commented)
      : comment_(_commented.comment()), value_(_commented.get()) {}

  /// Move constructor from Commented with compatible type.
  /// @tparam U Type compatible with T
  /// @param _commented The Commented to move from
  template <class U>
  Commented(Commented<U>&& _commented) noexcept
      : comment_(std::move(_commented.comment())),
        value_(std::move(_commented.value())) {}

  /// Copy constructor from Commented with convertible type.
  /// @tparam U Type convertible to T
  /// @param _commented The Commented to copy from
  template <class U>
    requires(std::is_convertible_v<U, Type>)
  Commented(const Commented<U>& _commented)
      : comment_(_commented.comment()), value_(_commented.value()) {}

  /// Constructs from any type convertible to Type (copy).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store
  template <class U>
    requires(std::is_convertible_v<U, Type>)
  Commented(const U& _value) : value_(_value) {}

  /// Constructs from any type convertible to Type (move).
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and store (will be moved)
  template <class U>
    requires(std::is_convertible_v<U, Type>)
  Commented(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  /// Assigns the underlying object to its default value using the Default
  /// sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  template <class U = Type>
    requires(std::is_default_constructible_v<U>)
  Commented(const Default&) : value_(Type()) {}

  /// Destructor.
  ~Commented() = default;

  /// Sets or updates the comment associated with the field.
  /// @param _comment The comment text to associate with this field
  void add_comment(std::string _comment) { comment_ = std::move(_comment); }

  /// Returns the comment associated with the field, if any.
  /// @return An optional containing the comment string, or nullopt if no
  /// comment
  const std::optional<std::string>& comment() const { return comment_; }

  /// Returns the underlying value.
  /// @return Reference to the stored value
  Type& get() { return value_; }

  /// Returns the underlying value (const).
  /// @return Const reference to the stored value
  const Type& get() const { return value_; }

  /// Function call operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator()() { return value_; }

  /// Function call operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator()() const { return value_; }

  /// Dereference operator - returns the underlying value.
  /// @return Reference to the stored value
  Type& operator*() { return value_; }

  /// Dereference operator (const) - returns the underlying value.
  /// @return Const reference to the stored value
  const Type& operator*() const { return value_; }

  /// Assigns a new value (preserves the comment).
  /// @param _value The value to assign
  /// @return Reference to this Commented
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns a new value (move version, preserves the comment).
  /// @param _value The value to assign (will be moved)
  /// @return Reference to this Commented
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns from any type convertible to the underlying type.
  /// @tparam U Type convertible to T
  /// @param _value The value to convert and assign
  /// @return Reference to this Commented
  template <class U>
    requires std::is_convertible_v<U, Type>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the value to its default using the Default sentinel.
  /// @tparam U The type (must be default constructible)
  /// @param The default sentinel value
  /// @return Reference to this Commented
  template <class U = Type>
    requires std::is_default_constructible_v<U>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Copy assignment operator.
  /// @param _commented The Commented to copy from
  /// @return Reference to this Commented
  Commented& operator=(const Commented& _commented) = default;

  /// Move assignment operator.
  /// @param _commented The Commented to move from
  /// @return Reference to this Commented
  Commented& operator=(Commented&& _commented) = default;

  /// Assigns from another Commented with compatible type (copy).
  /// Copies both the value and the comment.
  /// @tparam U Type compatible with T
  /// @param _commented The Commented to copy from
  /// @return Reference to this Commented
  template <class U>
  auto& operator=(const Commented<U>& _commented) {
    value_ = _commented.get();
    comment_ = _commented.comment();
    return *this;
  }

  /// Assigns from another Commented with compatible type (move).
  /// Moves both the value and the comment.
  /// @tparam U Type compatible with T
  /// @param _commented The Commented to move from
  /// @return Reference to this Commented
  template <class U>
  auto& operator=(Commented<U>&& _commented) {
    value_ = std::move(_commented.value_);
    comment_ = std::move(_commented.comment_);
    return *this;
  }

  /// Returns the underlying value for reflection (used by parsers).
  /// @return Const reference to the stored value
  const ReflectionType& reflection() const { return value_; }

  /// Sets the value (copy version).
  /// @param _value The value to set
  void set(const Type& _value) { value_ = _value; }

  /// Sets the value (move version).
  /// @param _value The value to set (will be moved)
  void set(Type&& _value) { value_ = std::move(_value); }

  /// Returns the underlying value.
  /// @return Reference to the stored value
  Type& value() { return value_; }

  /// Returns the underlying value (const).
  /// @return Const reference to the stored value
  const Type& value() const { return value_; }

  /// The comment associated with the field (if any).
  std::optional<std::string> comment_;

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
