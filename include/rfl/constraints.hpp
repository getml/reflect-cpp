#ifndef RFL_CONSTRAINTS_HPP_
#define RFL_CONSTRAINTS_HPP_

#include "rfl/Result.hpp"

namespace rfl {

template <int _threshold>
struct greater_than {
    template <class T>
    static Result<T> check(T _value) noexcept {
        if (_value <= static_cast<T>(_threshold)) {
            return Error("Value expected to be greater than " +
                         std::to_string(_threshold) + ", but got " +
                         std::to_string(_value) + ".");
        }
        return _value;
    }
};

template <int _threshold>
struct less_than {
    template <class T>
    static Result<T> check(T _value) noexcept {
        if (_value >= static_cast<T>(_threshold)) {
            return Error("Value expected to be less than " +
                         std::to_string(_threshold) + ", but got " +
                         std::to_string(_value) + ".");
        }
        return _value;
    }
};

/// Requires that all of the contraints C and Cs be true.
template <class C, class... Cs>
struct all_of {
    template <class T>
    static rfl::Result<T> check(T _value) noexcept {
        return check_impl<T, C, Cs...>(_value);
    }

   private:
    template <class T, class Head, class... Tail>
    static rfl::Result<T> check_impl(T _value) noexcept {
        if constexpr (sizeof...(Tail) == 0) {
            return Head::check(_value);
        } else {
            return Head::check(_value).and_then(check_impl<T, Tail...>);
        }
    }
};

/// Requires that any of the contraints C and Cs be true.
// ...

}  // namespace rfl

#endif
