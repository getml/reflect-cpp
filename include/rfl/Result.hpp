#ifndef RFL_RESULT_HPP_
#define RFL_RESULT_HPP_

#include <array>
#include <iostream>
#include <optional>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

#include "internal/is_array.hpp"
#include "internal/to_std_array.hpp"

namespace rfl {

/// To be returned
class Error {
 public:
  Error(const std::string& _what) : what_(_what) {}

  ~Error() = default;

  /// Returns the error message, equivalent to .what() in std::exception.
  const std::string& what() const { return what_; }

 private:
  /// Documents what went wrong
  std::string what_;
};

/// Can be used when we are simply interested in whether an operation was
/// successful.
struct Nothing {};

/// The Result class is used for monadic error handling.
template <class T>
class Result {
  static_assert(!std::is_same<T, Error>(), "The result type cannot be Error.");

  using TOrErr = std::array<unsigned char, std::max(sizeof(T), sizeof(Error))>;

 public:
  using Type = T;

  Result(const T& _val) : success_(true) { new (&get_t()) T(_val); }

  Result(T&& _val) noexcept : success_(true) {
    new (&get_t()) T(std::move(_val));
  }

  Result(const Error& _err) : success_(false) { new (&get_err()) Error(_err); }

  Result(Error&& _err) noexcept : success_(false) {
    new (&get_err()) Error(std::move(_err));
  }

  Result(Result<T>&& _other) noexcept : success_(_other.success_) {
    move_from_other(std::move(_other));
  }

  Result(const Result<T>& _other) : success_(_other.success_) {
    copy_from_other(_other);
  }

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Result(Result<U>&& _other)
      : success_(_other && true),
        t_or_err_(std::forward<Result<U>>(_other)
                      .transform([](U&& _u) { return T(std::forward<U>(_u)); })
                      .t_or_err_) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Result(const Result<U>& _other)
      : success_(_other && true),
        t_or_err_(
            _other.transform([](const U& _u) { return T(_u); }).t_or_err_) {}

  ~Result() { destroy(); }

  /// Returns Result<U>, if successful and error otherwise.
  /// Inspired by .and(...) in the Rust std::result type.
  template <class U>
  Result<U> and_other(const Result<U>& _r) const noexcept {
    const auto f = [&](const auto& _) { return _r; };
    return and_then(f);
  }

  /// Monadic operation - F must be a function of type T -> Result<U>.
  template <class F>
  auto and_then(const F& _f) {
    /// Result_U is expected to be of type Result<U>.
    using Result_U = typename std::invoke_result<F, T>::type;
    if (success_) {
      return Result_U(_f(std::forward<T>(get_t())));
    } else {
      return Result_U(std::forward<Error>(get_err()));
    }
  }

  /// Monadic operation - F must be a function of type T -> Result<U>.
  template <class F>
  auto and_then(const F& _f) const {
    /// Result_U is expected to be of type Result<U>.
    using Result_U = typename std::invoke_result<F, T>::type;
    if (success_) {
      return Result_U(_f(get_t()));
    } else {
      return Result_U(get_err());
    }
  }

  /// Results types can be iterated over, which even make it possible to use
  /// them within a std::range.
  T* begin() noexcept {
    if (success_) {
      return &get_t();
    } else {
      return nullptr;
    }
  }

  /// Results types can be iterated over, which even make it possible to use
  /// them within a std::range.
  const T* begin() const noexcept {
    if (success_) {
      return &get_t();
    } else {
      return nullptr;
    }
  }

  /// Results types can be iterated over, which even make it possible to use
  /// them within a std::range.
  T* end() noexcept {
    if (success_) {
      return &get_t() + 1;
    } else {
      return nullptr;
    }
  }

  /// Results types can be iterated over, which even make it possible to use
  /// them within a std::range.
  const T* end() const noexcept {
    if (success_) {
      return &get_t() + 1;
    } else {
      return nullptr;
    }
  }

  /// Returns an std::optional<error> if this does in fact contain an error
  /// or std::nullopt otherwise.
  std::optional<Error> error() const noexcept {
    if (success_) {
      return std::nullopt;
    } else {
      return get_err();
    }
  }

  /// Returns true if the result contains a value, false otherwise.
  operator bool() const noexcept { return success_; }

  /// Allows access to the underlying value. Careful: Will result in undefined
  /// behavior, if the result contains an error.
  T& operator*() noexcept { return get_t(); }

  /// Allows read access to the underlying value. Careful: Will result in
  /// undefined behavior, if the result contains an error.
  const T& operator*() const noexcept { return get_t(); }

  /// Assigns the underlying object.
  Result<T>& operator=(const Result<T>& _other) {
    if (this == &_other) {
      return *this;
    }
    destroy();
    success_ = _other.success_;
    copy_from_other(_other);
    return *this;
  }

  /// Assigns the underlying object.
  Result<T>& operator=(Result<T>&& _other) {
    if (this == &_other) {
      return *this;
    }
    destroy();
    success_ = _other.success_;
    move_from_other(std::move(_other));
    return *this;
  }

  /// Assigns the underlying object.
  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  auto& operator=(const Result<U>& _other) {
    const auto to_t = [](const U& _u) -> T { return _u; };
    t_or_err_ = _other.transform(to_t).t_or_err_;
    return *this;
  }

  /// Expects a function that takes of type Error -> Result<T> and returns
  /// Result<T>.
  template <class F>
  Result<T> or_else(const F& _f) {
    if (success_) {
      return std::forward<T>(get_t());
    } else {
      return _f(std::forward<Error>(get_err()));
    }
  }

  /// Expects a function that takes of type Error -> Result<T> and returns
  /// Result<T>.
  template <class F>
  Result<T> or_else(const F& _f) const {
    if (success_) {
      return get_t();
    } else {
      return _f(get_err());
    }
  }

  /// Returns the value contained if successful or the provided result r if
  /// not.
  Result<T> or_other(const Result<T>& _r) const noexcept {
    const auto f = [&](const auto& _) { return _r; };
    return or_else(f);
  }

  /// Functor operation - F must be a function of type T -> U.
  template <class F>
  auto transform(const F& _f) {
    /// Result_U is expected to be of type Result<U>.
    using U = typename std::invoke_result<F, T>::type;
    if (success_) {
      return rfl::Result<U>(_f(std::forward<T>(get_t())));
    } else {
      return rfl::Result<U>(std::forward<Error>(get_err()));
    }
  }

  /// Functor operation - F must be a function of type T -> U.
  template <class F>
  auto transform(const F& _f) const {
    /// Result_U is expected to be of type Result<U>.
    using U = typename std::invoke_result<F, T>::type;
    if (success_) {
      return rfl::Result<U>(_f(get_t()));
    } else {
      return rfl::Result<U>(get_err());
    }
  }

  /// Returns the value if the result does not contain an error, throws an
  /// exceptions if not. Similar to .unwrap() in Rust.
  T value() {
    if (success_) {
      return std::forward<T>(get_t());
    } else {
      throw std::runtime_error(get_err().what());
    }
  }

  /// Returns the value if the result does not contain an error, throws an
  /// exceptions if not. Similar to .unwrap() in Rust.
  const T& value() const {
    if (success_) {
      return get_t();
    } else {
      throw std::runtime_error(get_err().what());
    }
  }

  /// Returns the value or a default.
  T value_or(T&& _default) noexcept {
    if (success_) {
      return std::forward<T>(get_t());
    } else {
      return std::forward<T>(_default);
    }
  }

  /// Returns the value or a default.
  T value_or(const T& _default) const noexcept {
    if (success_) {
      return get_t();
    } else {
      return _default;
    }
  }

 private:
  template <class U>
  void call_destructor_on_array(const size_t _size, U* _ptr) const {
    for (size_t i = 0; i < _size; ++i) {
      if constexpr (std::is_array_v<U>) {
        call_destructor_on_array(sizeof(*_ptr) / sizeof(**_ptr), *(_ptr + i));
      } else if constexpr (internal::is_array_v<U>) {
        call_destructor_on_array(_ptr->arr_.size(), _ptr->arr_[i]);
      } else if constexpr (std::is_destructible_v<U>) {
        (_ptr + i)->~U();
      }
    }
  }

  template <class Target, class Source>
  void call_copy_constructor_on_array(Target* _t, Source* _s) const {
    if constexpr (std::is_const_v<Target>) {
      return call_copy_constructor_on_array(
          const_cast<std::remove_const_t<Target>*>(_t), _s);
    } else if constexpr (!internal::is_array_v<Source> &&
                         !std::is_array_v<Target>) {
      ::new (_t) Target(*_s);
    } else if constexpr (internal::is_array_v<Source>) {
      for (size_t i = 0; i < _s->arr_.size(); ++i) {
        call_copy_constructor_on_array(&((*_t)[i]), &(_s->arr_[i]));
      }
    } else {
      for (size_t i = 0; i < _s->size(); ++i) {
        call_copy_constructor_on_array(&((*_t)[i]), &((*_s)[i]));
      }
    }
  }

  template <class Target, class Source>
  void call_move_constructor_on_array(Target* _t, Source* _s) const {
    if constexpr (std::is_const_v<Target>) {
      return call_move_constructor_on_array(
          const_cast<std::remove_const_t<Target>*>(_t), _s);
    } else if constexpr (!internal::is_array_v<Source> &&
                         !std::is_array_v<Target>) {
      ::new (_t) Target(std::move(*_s));
    } else if constexpr (internal::is_array_v<Source>) {
      for (size_t i = 0; i < _s->arr_.size(); ++i) {
        call_move_constructor_on_array(&((*_t)[i]), &(_s->arr_[i]));
      }
    } else {
      for (size_t i = 0; i < _s->size(); ++i) {
        call_move_constructor_on_array(&((*_t)[i]), &((*_s)[i]));
      }
    }
  }

  void copy_from_other(const Result<T>& _other) {
    if (success_) {
      if constexpr (std::is_array_v<T> || internal::is_array_v<T>) {
        auto ptr = &get_t();
        call_copy_constructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
      } else if constexpr (std::is_destructible_v<T>) {
        new (&get_t()) T(_other.get_t());
      }
    } else {
      new (&get_err()) Error(_other.get_err());
    }
  }

  void destroy() {
    if (success_) {
      if constexpr (std::is_array_v<T>) {
        auto ptr = &get_t();
        call_destructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
      } else if constexpr (internal::is_array_v<T>) {
        auto ptr = &get_t();
        call_destructor_on_array(ptr->arr_.size(), &(ptr->arr_));
      } else if constexpr (std::is_destructible_v<T>) {
        get_t().~T();
      }
    } else {
      get_err().~Error();
    }
  }

  T& get_t() noexcept { return *(reinterpret_cast<T*>(t_or_err_.data())); }

  const T& get_t() const noexcept {
    return *(reinterpret_cast<const T*>(t_or_err_.data()));
  }

  Error& get_err() noexcept {
    return *(reinterpret_cast<Error*>(t_or_err_.data()));
  }

  const Error& get_err() const noexcept {
    return *(reinterpret_cast<const Error*>(t_or_err_.data()));
  }

  void move_from_other(Result<T>&& _other) {
    if (success_) {
      if constexpr (std::is_array_v<T> || internal::is_array_v<T>) {
        auto ptr = &get_t();
        call_move_constructor_on_array(sizeof(*ptr) / sizeof(**ptr), *ptr);
      } else if constexpr (std::is_destructible_v<T>) {
        new (&get_t()) T(std::move(_other.get_t()));
      }
    } else {
      new (&get_err()) Error(std::move(_other.get_err()));
    }
  }

 private:
  /// Signifies whether this was a success.
  bool success_;

  /// The underlying data, can either be T or Error.
  alignas(std::max(alignof(T), alignof(Error))) TOrErr t_or_err_;
};

}  // namespace rfl

#endif
