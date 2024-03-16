#ifndef RFL_RESULT_HPP_
#define RFL_RESULT_HPP_

#include <optional>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

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

 public:
  using Type = T;

  Result(const T& _val) : t_or_err_(_val) {}

  Result(T&& _val) noexcept : t_or_err_(std::move(_val)) {}

  Result(const Error& _err) : t_or_err_(_err) {}

  Result(Error&& _err) noexcept : t_or_err_(std::move(_err)) {}

  Result(Result<T>&& _other) noexcept = default;

  Result(const Result<T>& _other) = default;

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Result(Result<U>&& _other)
      : t_or_err_(std::forward<Result<U>>(_other)
                      .transform([](U&& _u) { return T(std::forward<U>(_u)); })
                      .t_or_err_) {}

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Result(const Result<U>& _other)
      : t_or_err_(
            _other.transform([](const U& _u) { return T(_u); }).t_or_err_) {}

  ~Result() = default;

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

    const auto handle_variant =
        [&]<class TOrError>(TOrError&& _t_or_err) -> Result_U {
      if constexpr (!std::is_same<std::remove_cvref_t<TOrError>, Error>()) {
        return _f(std::forward<TOrError>(_t_or_err));
      } else {
        return std::forward<TOrError>(_t_or_err);
      }
    };

    return std::visit(handle_variant,
                      std::forward<std::variant<T, Error>>(t_or_err_));
  }

  /// Monadic operation - F must be a function of type T -> Result<U>.
  template <class F>
  auto and_then(const F& _f) const {
    /// Result_U is expected to be of type Result<U>.
    using Result_U = typename std::invoke_result<F, T>::type;

    const auto handle_variant =
        [&]<class TOrError>(const TOrError& _t_or_err) -> Result_U {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return _f(_t_or_err);
      } else {
        return _t_or_err;
      }
    };

    return std::visit(handle_variant, t_or_err_);
  }

  /// Results types can be iterated over, which even make it possible to use
  /// them within a std::range.
  T* begin() noexcept {
    const auto get_ptr =
        [this]<class TOrError>(const TOrError& _t_or_err) -> T* {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return &_t_or_err;
      } else {
        return nullptr;
      }
    };
    return std::visit(get_ptr, t_or_err_);
  }

  /// Results types can be iterated over, which even make it possible to use
  /// them within a std::range.
  const T* begin() const noexcept {
    const auto get_ptr =
        [this]<class TOrError>(const TOrError& _t_or_err) -> const T* {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return &_t_or_err;
      } else {
        return nullptr;
      }
    };
    return std::visit(get_ptr, t_or_err_);
  }

  /// Results types can be iterated over, which even make it possible to use
  /// them within a std::range.
  T* end() noexcept {
    const auto get_ptr =
        [this]<class TOrError>(const TOrError& _t_or_err) -> T* {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return &_t_or_err + 1;
      } else {
        return nullptr;
      }
    };
    return std::visit(get_ptr, t_or_err_);
  }

  /// Results types can be iterated over, which even make it possible to use
  /// them within a std::range.
  const T* end() const noexcept {
    const auto get_ptr =
        []<class TOrError>(const TOrError& _t_or_err) -> const T* {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return &_t_or_err + 1;
      } else {
        return nullptr;
      }
    };
    return std::visit(get_ptr, t_or_err_);
  }

  /// Returns an std::optional<error> if this does in fact contain an error
  /// or std::nullopt otherwise.
  std::optional<Error> error() const noexcept {
    const auto get_err =
        []<class TOrError>(const TOrError& _t_or_err) -> std::optional<Error> {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return std::nullopt;
      } else {
        return _t_or_err;
      }
    };
    return std::visit(get_err, t_or_err_);
  }

  /// Returns true if the result contains a value, false otherwise.
  operator bool() const noexcept {
    return !std::holds_alternative<Error>(t_or_err_);
  }

  /// Allows access to the underlying value. Careful: Will result in undefined
  /// behavior, if the result contains an error.
  T& operator*() { return *std::get_if<T>(&t_or_err_); }

  /// Allows read access to the underlying value. Careful: Will result in
  /// undefined behavior, if the result contains an error.
  const T& operator*() const { return *std::get_if<T>(&t_or_err_); }

  /// Assigns the underlying object.
  Result<T>& operator=(const Result<T>& _other) = default;

  /// Assigns the underlying object.
  Result<T>& operator=(Result<T>&& _other) = default;

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
    const auto handle_variant =
        [&]<class TOrError>(TOrError&& _t_or_err) -> Result<T> {
      if constexpr (std::is_same<std::remove_cvref_t<TOrError>, Error>()) {
        return _f(std::forward<Error>(_t_or_err));
      } else {
        return std::forward<T>(_t_or_err);
      }
    };
    return std::visit(handle_variant,
                      std::forward<std::variant<T, Error>>(t_or_err_));
  }

  /// Expects a function that takes of type Error -> Result<T> and returns
  /// Result<T>.
  template <class F>
  Result<T> or_else(const F& _f) const {
    const auto handle_variant =
        [&_f]<class TOrError>(const TOrError& _t_or_err) -> Result<T> {
      if constexpr (std::is_same<TOrError, Error>()) {
        return _f(_t_or_err);
      } else {
        return _t_or_err;
      }
    };
    return std::visit(handle_variant, t_or_err_);
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

    const auto handle_variant =
        [&]<class TOrError>(TOrError&& _t_or_err) -> rfl::Result<U> {
      if constexpr (!std::is_same<std::remove_cvref_t<TOrError>, Error>()) {
        return _f(std::forward<TOrError>(_t_or_err));
      } else {
        return std::forward<TOrError>(_t_or_err);
      }
    };

    return std::visit(handle_variant,
                      std::forward<std::variant<T, Error>>(t_or_err_));
  }

  /// Functor operation - F must be a function of type T -> U.
  template <class F>
  auto transform(const F& _f) const {
    /// Result_U is expected to be of type Result<U>.
    using U = typename std::invoke_result<F, T>::type;

    const auto handle_variant =
        [&]<class TOrError>(const TOrError& _t_or_err) -> rfl::Result<U> {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return _f(_t_or_err);
      } else {
        return _t_or_err;
      }
    };

    return std::visit(handle_variant, t_or_err_);
  }

  /// Returns the value if the result does not contain an error, throws an
  /// exceptions if not. Similar to .unwrap() in Rust.
  T value() {
    const auto handle_variant = [&]<class TOrError>(TOrError& _t_or_err) -> T {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return std::forward<T>(_t_or_err);
      } else {
        throw std::runtime_error(_t_or_err.what());
      }
    };
    return std::visit(handle_variant, t_or_err_);
  }

  /// Returns the value if the result does not contain an error, throws an
  /// exceptions if not. Similar to .unwrap() in Rust.
  const T& value() const {
    const auto handle_variant =
        [&]<class TOrError>(const TOrError& _t_or_err) -> const T& {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return _t_or_err;
      } else {
        throw std::runtime_error(_t_or_err.what());
      }
    };
    return std::visit(handle_variant, t_or_err_);
  }

  /// Returns the value or a default.
  T value_or(T&& _default) noexcept {
    const auto handle_variant = [&]<class TOrError>(TOrError&& _t_or_err) -> T {
      using Type = std::remove_cvref_t<TOrError>;
      if constexpr (!std::is_same<Type, Error>()) {
        return std::forward<T>(_t_or_err);
      } else {
        return std::forward<T>(_default);
      }
    };
    return std::visit(handle_variant, t_or_err_);
  }

  /// Returns the value or a default.
  T value_or(const T& _default) const noexcept {
    const auto handle_variant =
        [&]<class TOrError>(const TOrError& _t_or_err) -> T {
      if constexpr (!std::is_same<TOrError, Error>()) {
        return _t_or_err;
      } else {
        return _default;
      }
    };
    return std::visit(handle_variant, t_or_err_);
  }

 private:
  /// The underlying variant, can either be T or Error.
  std::variant<T, Error> t_or_err_;
};

}  // namespace rfl

#endif
