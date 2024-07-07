#ifndef RFL_INTERNAL_SKIP_HPP_
#define RFL_INTERNAL_SKIP_HPP_

#include <optional>

namespace rfl::internal {

template <class T, bool _skip_serialization, bool _skip_deserialization>
class Skip {
 private:
  using SelfType = Skip<T, _skip_serialization, _skip_deserialization>;

 public:
  static constexpr bool skip_serialization_ = _skip_serialization;
  static constexpr bool skip_deserialization_ = _skip_deserialization;

  /// The underlying type.
  using Type = T;
  using ReflectionType = std::optional<T>;

  Skip() : value_(Type()) {}

  Skip(const Type& _value) : value_(_value) {}

  Skip(ReflectionType&& _value) noexcept
      : value_(_value ? std::move(*_value) : Type()) {}

  Skip(const ReflectionType& _value) : value_(_value ? *_value : Type()) {}

  Skip(Type&& _value) noexcept : value_(std::move(_value)) {}

  Skip(SelfType&& _skip) noexcept = default;

  Skip(const SelfType& _skip) = default;

  template <class U, bool _skip_s, bool _skip_d>
  Skip(const Skip<U, _skip_s, _skip_d>& _other) : value_(_other.get()) {}

  template <class U, bool _skip_s, bool _skip_d>
  Skip(Skip<U, _skip_s, _skip_d>&& _other) : value_(_other.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Skip(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Skip(U&& _value) noexcept : value_(std::forward<U>(_value)) {}

  template <class U, bool _skip_s, bool _skip_d,
            typename std::enable_if<std::is_convertible_v<U, Type>,
                                    bool>::type = true>
  Skip(const Skip<U, _skip_s, _skip_d>& _skip) : value_(_skip.value()) {}

  /// Assigns the underlying object to its default value.
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  Skip(const Default&) : value_(Type()) {}

  ~Skip() = default;

  /// Returns the underlying object.
  Type& get() { return value_; }

  /// Returns the underlying object.
  const Type& get() const { return value_; }

  /// Returns the underlying object.
  Type& operator()() { return value_; }

  /// Returns the underlying object.
  const Type& operator()() const { return value_; }

  /// Assigns the underlying object.
  auto& operator=(const Type& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object.
  auto& operator=(Type&& _value) noexcept {
    value_ = std::move(_value);
    return *this;
  }

  /// Assigns the underlying object.
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  auto& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object to its default value.
  template <class U = Type,
            typename std::enable_if<std::is_default_constructible_v<U>,
                                    bool>::type = true>
  auto& operator=(const Default&) {
    value_ = Type();
    return *this;
  }

  /// Assigns the underlying object.
  SelfType& operator=(const SelfType& _other) = default;

  /// Assigns the underlying object.
  SelfType& operator=(SelfType&& _other) = default;

  /// Assigns the underlying object.
  template <class U, bool _skip_s, bool _skip_d>
  auto& operator=(const Skip<U, _skip_s, _skip_d>& _skip) {
    value_ = _skip.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U, bool _skip_s, bool _skip_d>
  auto& operator=(Skip<U, _skip_s, _skip_d>&& _skip) {
    value_ = std::forward<T>(_skip.value_);
    return *this;
  }

  /// Returns the ReflectionType - necessary for the serialization to work.
  ReflectionType reflection() const { return value_; }

  /// Assigns the underlying object.
  void set(const Type& _value) { value_ = _value; }

  /// Assigns the underlying object.
  void set(Type&& _value) { value_ = std::move(_value); }

  /// Returns the underlying object.
  Type& value() { return value_; }

  /// Returns the underlying object.
  const Type& value() const { return value_; }

 private:
  /// The underlying value
  T value_;
};

}  // namespace rfl::internal

#endif
