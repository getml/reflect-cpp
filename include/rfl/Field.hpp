#ifndef RFL_FIELD_HPP_
#define RFL_FIELD_HPP_

#include <algorithm>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "rfl/Literal.hpp"
#include "rfl/default.hpp"
#include "rfl/internal/StringLiteral.hpp"

namespace rfl {

/// Used to define a field in the NamedTuple.
template <internal::StringLiteral _name, class _Type>
struct Field {
    /// The underlying type.
    using Type = _Type;

    /// The name of the field.
    using Name = rfl::Literal<_name>;

    Field(const Type& _value) : value_(_value) {}

    Field(Type&& _value) noexcept : value_(std::move(_value)) {}

    Field(Field<_name, _Type>&& _field) noexcept
        : value_(std::move(_field.value_)) {}

    Field(const Field<_name, Type>& _field) : value_(_field.get()) {}

    template <class T>
    Field(const Field<_name, T>& _field) : value_(_field.get()) {}

    template <class T>
    Field(Field<_name, T>&& _field) : value_(_field.get()) {}

    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    Field(const T& _value) : value_(_value) {}

    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    Field(T&& _value) noexcept : value_(std::forward<T>(_value)) {}

    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    Field(const Field<_name, T>& _field) : value_(_field.value()) {}

    /// Assigns the underlying object to its default value.
    template <class T = Type,
              typename std::enable_if<std::is_default_constructible_v<T>,
                                      bool>::type = true>
    Field(const Default& _default) : value_(Type()) {}

    ~Field() = default;

    /// The name of the field, for internal use.
    constexpr static const internal::StringLiteral name_ = _name;

    /// Returns the underlying object.
    inline const Type& get() const { return value_; }

    /// Returns the underlying object.
    inline Type& operator()() { return value_; }

    /// Returns the underlying object.
    inline const Type& operator()() const { return value_; }

    /// Assigns the underlying object.
    inline auto& operator=(const Type& _value) {
        value_ = _value;
        return *this;
    }

    /// Assigns the underlying object.
    inline auto& operator=(Type&& _value) noexcept {
        value_ = std::move(_value);
        return *this;
    }

    /// Assigns the underlying object.
    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    inline auto& operator=(const T& _value) {
        value_ = _value;
        return *this;
    }

    /// Assigns the underlying object to its default value.
    template <class T = Type,
              typename std::enable_if<std::is_default_constructible_v<T>,
                                      bool>::type = true>
    inline auto& operator=(const Default& _default) {
        value_ = Type();
        return *this;
    }
    /// Assigns the underlying object.
    inline Field<_name, _Type>& operator=(const Field<_name, _Type>& _field) {
        if (&_field != this) {
            value_ = _field.get();
        }
        return *this;
    }

    /// Assigns the underlying object.
    inline Field<_name, _Type>& operator=(Field<_name, _Type>&& _field) {
        if (&_field != this) {
            value_ = std::move(_field.value_);
        }
        return *this;
    }

    /// Assigns the underlying object.
    template <class T>
    inline auto& operator=(const Field<_name, T>& _field) {
        value_ = _field.get();
        return *this;
    }

    /// Assigns the underlying object.
    template <class T>
    inline auto& operator=(Field<_name, T>&& _field) {
        value_ = std::forward<T>(_field.value_);
        return *this;
    }

    /// Assigns the underlying object.
    inline void set(const Type& _value) { value_ = _value; }

    /// Assigns the underlying object.
    inline void set(Type&& _value) { value_ = std::move(_value); }

    /// Returns the underlying object.
    inline Type& value() { return value_; }

    /// Returns the underlying object.
    inline const Type& value() const { return value_; }

    /// The underlying value.
    Type value_;
};

template <internal::StringLiteral _name, class _Type>
inline auto make_field(_Type&& _value) {
    return Field<_name, _Type>(std::forward<_Type>(_value));
}

template <internal::StringLiteral _name, class _Type>
inline auto make_field(const _Type& _value) {
    return Field<_name, _Type>(_value);
}

}  // namespace rfl

#endif  // RFL_FIELD_HPP_
