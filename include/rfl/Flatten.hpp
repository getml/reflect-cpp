#ifndef RFL_FLATTEN_HPP_
#define RFL_FLATTEN_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace rfl {

/// Used to embed another struct into the generated output.
template <class _Type>
struct Flatten {
    /// The underlying type.
    using Type = std::decay_t<_Type>;

    Flatten(const Type& _value) : value_(_value) {}

    Flatten(Type&& _value) noexcept : value_(std::forward<Type>(_value)) {}

    Flatten(const Flatten<_Type>& _f) = default;

    Flatten(Flatten<_Type>&& _f) noexcept = default;

    template <class T>
    Flatten(const Flatten<T>& _f) : value_(_f.get()) {}

    template <class T>
    Flatten(Flatten<T>&& _f) : value_(_f.get()) {}

    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    Flatten(const T& _value) : value_(_value) {}

    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    Flatten(T&& _value) : value_(_value) {}

    ~Flatten() = default;

    /// Returns the underlying object.
    const Type& get() const { return value_; }

    /// Returns the underlying object.
    Type& operator()() { return value_; }

    /// Returns the underlying object.
    const Type& operator()() const { return value_; }

    /// Assigns the underlying object.
    Flatten<_Type>& operator=(const _Type& _value) {
        value_ = _value;
        return *this;
    }

    /// Assigns the underlying object.
    Flatten<_Type>& operator=(_Type&& _value) {
        value_ = std::forward<Type>(_value);
        return *this;
    }

    /// Assigns the underlying object.
    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    Flatten<_Type>& operator=(const T& _value) {
        value_ = _value;
        return *this;
    }

    /// Assigns the underlying object.
    Flatten<_Type>& operator=(const Flatten<_Type>& _f) = default;

    /// Assigns the underlying object.
    Flatten<_Type>& operator=(Flatten<_Type>&& _f) = default;
    /// Assigns the underlying object.
    template <class T>
    Flatten<_Type>& operator=(const Flatten<T>& _f) {
        value_ = _f.get();
        return *this;
    }

    /// Assigns the underlying object.
    template <class T>
    Flatten<_Type>& operator=(Flatten<T>&& _f) {
        value_ = std::forward<T>(_f);
        return *this;
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
