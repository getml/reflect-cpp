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

    Flatten(Type&& _value) : value_(std::forward<Type>(_value)) {}

    Flatten(const Flatten<Type>& _f) : value_(_f.get()) {}

    Flatten(Flatten<Type>&& _f) : value_(std::forward<Type>(_f.value_)) {}

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
    inline const Type& get() const { return value_; }

    /// Returns the underlying object.
    inline Type& operator()() { return value_; }

    /// Returns the underlying object.
    inline const Type& operator()() const { return value_; }

    /// Assigns the underlying object.
    inline Flatten<Type> operator=(const Type& _value) {
        value_ = _value;
        return *this;
    }

    /// Assigns the underlying object.
    inline Flatten<Type> operator=(Type&& _value) {
        value_ = std::forward<Type>(_value);
        return *this;
    }

    /// Assigns the underlying object.
    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    inline void operator=(const T& _value) {
        value_ = _value;
    }

    /// Assigns the underlying object.
    inline void operator=(const Flatten<Type>& _f) { value_ = _f.get(); }

    /// Assigns the underlying object.
    inline void operator=(Flatten<Type>&& _f) {
        value_ = std::forward<Type>(_f);
    }

    /// Assigns the underlying object.
    template <class T>
    inline void operator=(const Flatten<T>& _f) {
        value_ = _f.get();
    }

    /// Assigns the underlying object.
    template <class T>
    inline void operator=(Flatten<T>&& _f) {
        value_ = std::forward<T>(_f);
    }

    /// Assigns the underlying object.
    inline void set(const Type& _value) { value_ = _value; }

    /// Assigns the underlying object.
    inline void set(Type&& _value) { value_ = std::forward<Type>(_value); }

    /// The underlying value.
    Type value_;
};

}  // namespace rfl

#endif
