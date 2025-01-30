#ifndef RFL_RESULT_HPP_
#define RFL_RESULT_HPP_

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>

/*
  Implementation Common to both
*/
namespace rfl {
class Error {
 public:
  Error(const std::string& _what) : what_(_what) {}
  Error(const Error& e) = default;
  Error& operator=(const Error&) = default;

  /// Returns the error message, equivalent to .what() in std::exception.
  const std::string& what() const { return what_; }

 private:
  /// Documents what went wrong
  std::string what_;
};
struct Nothing {};
}  // namespace rfl
/*
  This implementation are for C++ parts with std::expected defined
*/
#ifdef __cpp_lib_expected
#include <expected>
namespace rfl {
using Unexpected = std::unexpected<Error>;
template <class T>
using Result = std::expected<T, rfl::Error>;
}  // namespace rfl
#else
namespace rfl {
template <class E>
struct Unexpected {
  Unexpected(E&& _err) : err_{std::forward<E>(_err)} {}
  Unexpected(const E& _err) : err_{_err} {}
  Unexpected(Unexpected&&) = default;
  Unexpected(const Unexpected&) = default;
  Unexpected& operator=(Unexpected&&) = default;
  Unexpected& operator=(const Unexpected&) = default;
  const E& error() const& { return err_; }
  E&& error() && { return std::move(err_); }
  E& error() & { return err_; }

 private:
  E err_;
};
/// The Result class is used for monadic error handling.
template <class T>
class Result {
  static_assert(!std::is_same<T, Error>(), "The result type cannot be Error.");

  using TOrErr = std::array<unsigned char, std::max(sizeof(T), sizeof(Error))>;

 public:
  // using Type = T;
  using value_type = T;
  using error_type = rfl::Error;

  Result(const T& _val) : success_(true) { new (&get_t()) T(_val); }

  Result(T&& _val) noexcept : success_(true) {
    new (&get_t()) T(std::move(_val));
  }

  Result(const Unexpected<Error>& _err) : success_(false) {
    new (&get_err()) Error(_err.error());
  }
  Result(Unexpected<Error>&& _err) : success_(false) {
    new (&get_err()) Error(std::move(_err.error()));
  }

  // Result(Error&& _err) noexcept : success_(false) {
  //   new (&get_err()) Error(std::move(_err));
  // }

  Result(Result<T>&& _other) noexcept : success_(_other.success_) {
    move_from_other(_other);
  }

  Result(const Result<T>& _other) : success_(_other.success_) {
    copy_from_other(_other);
  }

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Result(Result<U>&& _other) : success_(_other && true) {
    auto temp = std::forward<Result<U> >(_other).transform(
        [](U&& _u) { return T(std::forward<U>(_u)); });
    move_from_other(temp);
  }

  template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                             bool>::type = true>
  Result(const Result<U>& _other) : success_(_other && true) {
    auto temp = _other.transform([](const U& _u) { return T(_u); });
    move_from_other(temp);
  }

  ~Result() { destroy(); }

  /*
    Not specified in the standard.
  */
  // /// Returns Result<U>, if successful and error otherwise.
  // /// Inspired by .and(...) in the Rust std::result type.
  // template <class U>
  // Result<U> and_other(const Result<U>& _r) const noexcept {
  //   const auto f = [&](const auto& _) { return _r; };
  //   return and_then(f);
  // }

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

  /*
    Not specified in the standard
  */
  // /// Results types can be iterated over, which even make it possible to use
  // /// them within a std::range.
  // T* begin() noexcept {
  //   if (success_) {
  //     return &get_t();
  //   } else {
  //     return nullptr;
  //   }
  // }

  // /// Results types can be iterated over, which even make it possible to use
  // /// them within a std::range.
  // const T* begin() const noexcept {
  //   if (success_) {
  //     return &get_t();
  //   } else {
  //     return nullptr;
  //   }
  // }

  // /// Results types can be iterated over, which even make it possible to use
  // /// them within a std::range.
  // T* end() noexcept {
  //   if (success_) {
  //     return &get_t() + 1;
  //   } else {
  //     return nullptr;
  //   }
  // }

  // /// Results types can be iterated over, which even make it possible to use
  // /// them within a std::range.
  // const T* end() const noexcept {
  //   if (success_) {
  //     return &get_t() + 1;
  //   } else {
  //     return nullptr;
  //   }
  // }

  /// Returns an std::optional<error> if this does in fact contain an error
  /// or std::nullopt otherwise.

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
  Result<T>& operator=(Result<T>&& _other) noexcept {
    if (this == &_other) {
      return *this;
    }
    destroy();
    success_ = _other.success_;
    move_from_other(_other);
    return *this;
  }

  Result<T>& operator=(Unexpected<Error>&& _err) noexcept {
    destroy();
    success_ = false;
    new (&get_err()) Error(_err.error());
    return *this;
  }
  Result<T>& operator=(const Unexpected<Error>& _err) noexcept {
    destroy();
    success_ = false;
    new (&get_err()) Error(_err.error());
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

  /*
    Not specified in the standard.
    https://en.cppreference.com/w/cpp/utility/expected
  */
  /// Returns the value contained if successful or the provided result r if
  /// not.
  // Result<T> or_other(const Result<T>& _r) const noexcept {
  //   const auto f = [&](const auto& _) { return _r; };
  //   return or_else(f);
  // }

  /// Functor operation - F must be a function of type T -> U.
  template <class F>
  auto transform(const F& _f) {
    /// Result_U is expected to be of type Result<U>.
    using U = typename std::invoke_result<F, T>::type;
    if (success_) {
      return rfl::Result<U>(_f(std::forward<T>(get_t())));
    } else {
      return rfl::Result<U>(rfl::Unexpected(std::forward<Error>(get_err())));
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
  T& value() {
    if (success_) {
      return get_t();
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

  /*
    As specified by the standard :
    https://en.cppreference.com/w/cpp/utility/expected
    Observers
  */
  template <class G = rfl::Error>
  rfl::Error error_or(G&& _default) const& {
    if (success_) {
      return _default;
    } else {
      return get_err();
    }
  }
  template <class G = rfl::Error>
  rfl::Error error_or(G&& _default) && {
    if (success_) {
      return _default;
    } else {
      return get_err();
    }
  }
  bool has_value() const noexcept { return success_; }
  /*
    This 2 function definitions are as written by the documentation specified in
    https://en.cppreference.com/w/cpp/utility/expected/error
    and will have undefined behaviour if the Result does not contain an error.

    Not sure if the move overload should be included
    This exception based behaviour shall be retained to avoid weird bugs from
    happening without being able to trace source.
  */
  const Error& error() const& {
    if (success_) throw std::runtime_error("Expected does not contain value");
    return get_err();
  }
  Error& error() & {
    if (success_) throw std::runtime_error("Expected does not contain value");
    return get_err();
  }
  /*
    Read access to the value inside. As specified by :
    https://en.cppreference.com/w/cpp/utility/expected
    - Observers
  */
  T* operator->() noexcept { return &get_t(); }
  const T* operator->() const noexcept { return &get_t(); }
  /*
    transform_error
  */
  template <class F>
  rfl::Result<T> transform_error(F&& f) && {
    static_assert(
        std::same_as<std::invoke_result_t<F, rfl::Error>, rfl::Error>);
    if (!has_value()) {
      return rfl::Result<T>{std::invoke(f, std::move(get_err()))};
    } else {
      return rfl::Result<T>{std::move(value())};
    }
  }
  template <class F>
  rfl::Result<T> transform_error(F&& f) const& {
    static_assert(
        std::same_as<std::invoke_result_t<F, rfl::Error>, rfl::Error>);
    if (!has_value()) {
      return rfl::Result<T>{std::invoke(f, get_err())};
    } else {
      return rfl::Result<T>{value()};
    }
  }

 private:
  void copy_from_other(const Result<T>& _other) {
    if (success_) {
      new (&get_t()) T(_other.get_t());
    } else {
      new (&get_err()) Error(_other.get_err());
    }
  }

  void destroy() {
    if (success_) {
      if constexpr (std::is_destructible_v<std::remove_cv_t<T> >) {
        get_t().~T();
      }
    } else {
      get_err().~Error();
    }
  }

  T& get_t() noexcept {
    return *std::launder((reinterpret_cast<T*>(t_or_err_.data())));
  }

  const T& get_t() const noexcept {
    return *std::launder((reinterpret_cast<const T*>(t_or_err_.data())));
  }

  Error& get_err() noexcept {
    return *std::launder((reinterpret_cast<Error*>(t_or_err_.data())));
  }

  const Error& get_err() const noexcept {
    return *std::launder((reinterpret_cast<const Error*>(t_or_err_.data())));
  }

  void move_from_other(Result<T>& _other) noexcept {
    if (success_) {
      new (&get_t()) T(std::move(_other.get_t()));
    } else {
      new (&get_err()) Error(std::move(_other.get_err()));
    }
  }
  /// Signifies whether this was a success.
  bool success_;

  /// The underlying data, can either be T or Error.
  alignas(std::max(alignof(T), alignof(Error))) TOrErr t_or_err_;
};

}  // namespace rfl
#endif
#endif
