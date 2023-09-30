#ifndef RFL_RESULT_HPP_
#define RFL_RESULT_HPP_

#include <optional>
#include <ranges>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>

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

/// Helper class to be used to within the Result class to support the operator*.
/// This is necessary, because we only want this class to be unwrapped, not
/// normal std::tuple.
template <class... Args>
struct HelperTuple {
    using TupleType = std::tuple<Args...>;
    TupleType tuple_;
};

template <class>
struct is_helper_tuple : std::false_type {};

template <class... Args>
struct is_helper_tuple<HelperTuple<Args...>> : std::true_type {};

template <class T>
struct value_type {
    using Type = T;
};

template <class... Args>
struct value_type<HelperTuple<Args...>> {
    using Type = HelperTuple<Args...>::TupleType;
};

/// The Result class is used for monadic error handling.
template <class T>
class Result {
    static_assert(!std::is_same<T, Error>(),
                  "The result type cannot be Error.");

   public:
    using ValueType = typename value_type<T>::Type;

    Result(const T& _val) : t_or_err_(_val) {}

    Result(T&& _val) : t_or_err_(_val) {}

    Result(const Error& _err) : t_or_err_(_err) {}

    Result(Error&& _err) : t_or_err_(_err) {}

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
        const auto apply = [&_f](auto&& _t) {
            if constexpr (is_helper_tuple<T>()) {
                using TupleType = std::decay_t<decltype(_t.tuple)>;
                return std::apply(_f, std::forward<TupleType>(_t.tuple_));
            } else {
                return _f(_t);
            }
        };

        /// Result_U is expected to be of type Result<U>.
        using Result_U = typename std::invoke_result<decltype(apply), T>::type;

        const auto handle_variant =
            [apply]<class TOrError>(TOrError&& _t_or_err) -> Result_U {
            if constexpr (!std::is_same<std::decay_t<TOrError>, Error>()) {
                return apply(std::forward<TOrError>(_t_or_err));
            } else {
                return _t_or_err;
            }
        };

        return std::visit(handle_variant, t_or_err_);
    }

    /// Monadic operation - F must be a function of type T -> Result<U>.
    template <class F>
    auto and_then(const F& _f) const {
        const auto apply = [&_f](const auto& _t) {
            if constexpr (is_helper_tuple<T>()) {
                return std::apply(_f, _t.tuple_);
            } else {
                return _f(_t);
            }
        };

        /// Result_U is expected to be of type Result<U>.
        using Result_U = typename std::invoke_result<decltype(apply), T>::type;

        const auto handle_variant =
            [apply]<class TOrError>(const TOrError& _t_or_err) -> Result_U {
            if constexpr (!std::is_same<TOrError, Error>()) {
                return apply(_t_or_err);
            } else {
                return _t_or_err;
            }
        };

        return std::visit(handle_variant, t_or_err_);
    }

    /// Results types can be iterated over, which even make it possible to use
    /// them within a std::range.
    ValueType* begin() noexcept {
        const auto get_ptr =
            [this]<class TOrError>(const TOrError& _t_or_err) -> ValueType* {
            if constexpr (!std::is_same<TOrError, Error>()) {
                if constexpr (is_helper_tuple<T>()) {
                    return &_t_or_err.tuple_;
                } else {
                    return &_t_or_err;
                }
            } else {
                return nullptr;
            }
        };
        return std::visit(get_ptr, t_or_err_);
    }

    /// Results types can be iterated over, which even make it possible to use
    /// them within a std::range.
    const ValueType* begin() const noexcept {
        const auto get_ptr =
            [this]<class TOrError>(
                const TOrError& _t_or_err) -> const ValueType* {
            if constexpr (!std::is_same<TOrError, Error>()) {
                if constexpr (is_helper_tuple<T>()) {
                    return &_t_or_err.tuple_;
                } else {
                    return &_t_or_err;
                }
            } else {
                return nullptr;
            }
        };
        return std::visit(get_ptr, t_or_err_);
    }

    /// Results types can be iterated over, which even make it possible to use
    /// them within a std::range.
    ValueType* end() noexcept {
        const auto get_ptr =
            [this]<class TOrError>(const TOrError& _t_or_err) -> ValueType* {
            if constexpr (!std::is_same<TOrError, Error>()) {
                if constexpr (is_helper_tuple<T>()) {
                    return &_t_or_err.tuple_ + 1;
                } else {
                    return &_t_or_err + 1;
                }
            } else {
                return nullptr;
            }
        };
        return std::visit(get_ptr, t_or_err_);
    }

    /// Results types can be iterated over, which even make it possible to use
    /// them within a std::range.
    const ValueType* end() const noexcept {
        const auto get_ptr =
            [this]<class TOrError>(
                const TOrError& _t_or_err) -> const ValueType* {
            if constexpr (!std::is_same<TOrError, Error>()) {
                if constexpr (is_helper_tuple<T>()) {
                    return &_t_or_err.tuple_ + 1;
                } else {
                    return &_t_or_err + 1;
                }
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
            [this]<class TOrError>(
                const TOrError& _t_or_err) -> std::optional<Error> {
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
    template <class U, typename std::enable_if<std::is_convertible_v<U, T>,
                                               bool>::type = true>
    inline void operator=(const Result<U>& _other) {
        const auto to_t = [](const U& _u) -> T { return _u; };
        t_or_err_ = _other.transform(to_t).t_or_err_;
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
        const auto apply = [&_f](auto&& _t) {
            if constexpr (is_helper_tuple<T>()) {
                using TupleType = std::decay_t<decltype(_t.tuple)>;
                return std::apply(_f, std::forward<TupleType>(_t.tuple_));
            } else {
                return _f(std::forward<T>(_t));
            }
        };

        /// Result_U is expected to be of type Result<U>.
        using U = typename std::invoke_result<decltype(apply), T>::type;

        const auto handle_variant =
            [apply]<class TOrError>(TOrError&& _t_or_err) -> rfl::Result<U> {
            if constexpr (!std::is_same<std::decay_t<TOrError>, Error>()) {
                return apply(std::forward<TOrError>(_t_or_err));
            } else {
                return _t_or_err;
            }
        };

        return std::visit(handle_variant, t_or_err_);
    }

    /// Functor operation - F must be a function of type T -> U.
    template <class F>
    auto transform(const F& _f) const {
        const auto apply = [&_f](const auto& _t) {
            if constexpr (is_helper_tuple<T>()) {
                return std::apply(_f, _t.tuple_);
            } else {
                return _f(_t);
            }
        };

        /// Result_U is expected to be of type Result<U>.
        using U = typename std::invoke_result<decltype(apply), T>::type;

        const auto handle_variant =
            [apply]<class TOrError>(
                const TOrError& _t_or_err) -> rfl::Result<U> {
            if constexpr (!std::is_same<TOrError, Error>()) {
                return apply(_t_or_err);
            } else {
                return _t_or_err;
            }
        };

        return std::visit(handle_variant, t_or_err_);
    }

    /// Returns the value if the result does not contain an error, throws an
    /// exceptions if not. Similar to .unwrap() in Rust.
    ValueType& value() {
        const auto handle_variant =
            [&]<class TOrError>(TOrError& _t_or_err) -> ValueType& {
            if constexpr (!std::is_same<TOrError, Error>()) {
                if constexpr (is_helper_tuple<T>()) {
                    return _t_or_err.tuple_;
                } else {
                    return _t_or_err;
                }
            } else {
                throw std::runtime_error(_t_or_err.what());
            }
        };
        return std::visit(handle_variant, t_or_err_);
    }

    /// Returns the value if the result does not contain an error, throws an
    /// exceptions if not. Similar to .unwrap() in Rust.
    const ValueType& value() const {
        const auto handle_variant =
            [&]<class TOrError>(const TOrError& _t_or_err) -> const ValueType& {
            if constexpr (!std::is_same<TOrError, Error>()) {
                if constexpr (is_helper_tuple<T>()) {
                    return _t_or_err.tuple_;
                } else {
                    return _t_or_err;
                }
            } else {
                throw std::runtime_error(_t_or_err.what());
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

/// operator* allows to combine result types as a product type.

template <class T, class U>
inline auto operator*(const Result<T>& _rt, const Result<U>& _ru) {
    const auto f1 = [&_ru](const T& _t) {
        const auto f2 = [&_t](const U& _u) {
            return HelperTuple<T, U>(std::make_tuple(_t, _u));
        };
        return _ru.transform(f2);
    };
    return _rt.and_then(f1);
}

template <class... T, class... U>
inline auto operator*(const Result<HelperTuple<T...>>& _rt,
                      const Result<HelperTuple<U...>>& _ru) {
    const auto f1 = [&_ru](const T&... _t) {
        const auto f2 = [&](const U&... _u) {
            return HelperTuple<T..., U...>(std::make_tuple(_t..., _u...));
        };
        return _ru.transform(f2);
    };
    return _rt.and_then(f1);
}

template <class... T, class U>
inline auto operator*(const Result<const HelperTuple<T...>>& _rt,
                      const Result<U>& _ru) {
    const auto f = [](const U& _u) {
        return HelperTuple<U>(std::make_tuple(_u));
    };
    return _rt * _ru.transform(f);
}

template <class T, class... U>
inline auto operator*(const Result<T>& _rt,
                      const Result<HelperTuple<U...>>& _ru) {
    const auto f = [](const T& _t) {
        return HelperTuple<T>(std::make_tuple(_t));
    };
    return _rt.transform(f) * _ru;
}

template <class T, class U>
inline auto operator*(const Result<T>& _rt, const U& _u) {
    return _rt * Result<U>(_u);
}

template <class T, class U>
inline auto operator*(const T& _t, const Result<U>& _ru) {
    return Result<T>(_t) * _ru;
}

template <class T, class U>
inline auto operator*(Result<T>&& _rt, Result<U>&& _ru) {
    const auto f1 = [&_ru](T&& _t) {
        const auto f2 = [&_t](U&& _u) {
            return HelperTuple<T, U>(std::forward_as_tuple(_t, _u));
        };
        return _ru.transform(f2);
    };
    return _rt.and_then(f1);
}

template <class... T, class... U>
inline auto operator*(Result<HelperTuple<T...>>&& _rt,
                      Result<HelperTuple<U...>>&& _ru) {
    const auto f1 = [&_ru](T&&... _t) {
        const auto f2 = [&](U&&... _u) {
            return HelperTuple<T..., U...>(std::forward_as_tuple(_t..., _u...));
        };
        return _ru.transform(f2);
    };
    return _rt.and_then(f1);
}

template <class... T, class U>
inline auto operator*(Result<HelperTuple<T...>>&& _rt, Result<U>&& _ru) {
    const auto f = [](U&& _u) {
        return HelperTuple<U>(std::forward_as_tuple(_u));
    };
    return _rt * _ru.transform(f);
}

template <class T, class... U>
inline auto operator*(Result<T>&& _rt, Result<HelperTuple<U...>>&& _ru) {
    const auto f = [](T&& _t) {
        return HelperTuple<T>(std::forward_as_tuple(_t));
    };
    return _rt.transform(f) * _ru;
}

template <class T, class U>
inline auto operator*(Result<T>&& _rt, U&& _u) {
    return _rt * Result<U>(_u);
}

template <class T, class U>
inline auto operator*(T&& _t, Result<U>&& _ru) {
    return Result<T>(_t) * _ru;
}

}  // namespace rfl

#endif

