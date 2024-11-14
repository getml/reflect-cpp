#ifndef RFL_FLATTEN_HPP_
#define RFL_FLATTEN_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rfl {

/// Used to embed another struct into the generated output.
template <class T>
struct Flatten {
  /// The underlying type.
  using Type = std::remove_cvref_t<T>;

  Flatten() = default;

  Flatten(const Type& _value) : value_(_value) {}

  Flatten(Type&& _value) noexcept : value_(std::forward<Type>(_value)) {}

  Flatten(const Flatten<T>& _f) = default;

  Flatten(Flatten<T>&& _f) noexcept = default;

  template <class U>
  Flatten(const Flatten<U>& _f) : value_(_f.get()) {}

  template <class U>
  Flatten(Flatten<U>&& _f) : value_(_f.get()) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Flatten(const U& _value) : value_(_value) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Flatten(U&& _value) : value_(_value) {}

  ~Flatten() = default;

  /// Returns the underlying object.
  Type& get() { return value_; }

  /// Returns the underlying object.
  const Type& get() const { return value_; }

  /// Returns the underlying object.
  Type& operator()() { return value_; }

  /// Returns the underlying object.
  const Type& operator()() const { return value_; }

  /// Assigns the underlying object.
  Flatten<T>& operator=(const T& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object.
  Flatten<T>& operator=(T&& _value) {
    value_ = std::forward<Type>(_value);
    return *this;
  }

  /// Assigns the underlying object.
  template <class U, typename std::enable_if<std::is_convertible_v<U, Type>,
                                             bool>::type = true>
  Flatten<T>& operator=(const U& _value) {
    value_ = _value;
    return *this;
  }

  /// Assigns the underlying object.
  Flatten<T>& operator=(const Flatten<T>& _f) = default;

  /// Assigns the underlying object.
  Flatten<T>& operator=(Flatten<T>&& _f) = default;

  /// Assigns the underlying object.
  template <class U>
  Flatten<T>& operator=(const Flatten<U>& _f) {
    value_ = _f.get();
    return *this;
  }

  /// Assigns the underlying object.
  template <class U>
  Flatten<T>& operator=(Flatten<U>&& _f) {
    value_ = std::forward<U>(_f);
    return *this;
  }

  /// Three-way comparison operator
  template <class U>
  auto operator<=>(const Flatten<U>& _f) const {
    return value_ <=> _f.value_;
  }

  /// Equality comparison operator.
  template <class U>
  bool operator==(const Flatten<U>& _f) const {
    return value_ == _f.get();
  }

  /// Assigns the underlying object.
  void set(const Type& _value) { value_ = _value; }

  /// Assigns the underlying object.
  void set(Type&& _value) { value_ = std::forward<Type>(_value); }

  /// The underlying value.
  Type value_;
};

}  // namespace rfl

#endif
