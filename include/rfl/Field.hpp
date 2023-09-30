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
    using Type = std::decay_t<_Type>;

    /// The name of the field.
    using Name = rfl::Literal<_name>;

    Field(const Type& _value) : value_(_value) {}

    Field(Type&& _value) : value_(std::forward<Type>(_value)) {}

    Field(const Field<_name, Type>& _field) : value_(_field.get()) {}

    Field(Field<_name, Type>&& _field)
        : value_(std::forward<Type>(_field.value_)) {}

    template <class T>
    Field(const Field<_name, T>& _field) : value_(_field.get()) {}

    template <class T>
    Field(Field<_name, T>&& _field) : value_(_field.get()) {}

    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    Field(const T& _value) : value_(_value) {}

    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    Field(T&& _value) : value_(_value) {}

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
    inline void operator=(const Type& _value) { value_ = _value; }

    /// Assigns the underlying object.
    inline void operator=(Type&& _value) {
        value_ = std::forward<Type>(_value);
    }

    /// Assigns the underlying object.
    template <class T, typename std::enable_if<std::is_convertible_v<T, Type>,
                                               bool>::type = true>
    inline void operator=(const T& _value) {
        value_ = _value;
    }

    /// Assigns the underlying object to its default value.
    template <class T = Type,
              typename std::enable_if<std::is_default_constructible_v<T>,
                                      bool>::type = true>
    inline void operator=(const Default& _default) {
        value_ = Type();
    }

    /// Assigns the underlying object.
    inline void operator=(const Field<_name, Type>& _field) {
        value_ = _field.get();
    }

    /// Assigns the underlying object.
    inline void operator=(Field<_name, Type>&& _field) {
        value_ = _field.get();
    }

    /// Assigns the underlying object.
    template <class T>
    inline void operator=(const Field<_name, T>& _field) {
        value_ = _field.get();
    }

    /// Assigns the underlying object.
    template <class T>
    inline void operator=(Field<_name, T>&& _field) {
        value_ = _field.get();
    }

    /// Assigns the underlying object.
    inline void set(const Type& _value) { value_ = _value; }

    /// Assigns the underlying object.
    inline void set(Type&& _value) { value_ = std::forward<Type>(_value); }

    /// Returns the underlying object.
    inline Type& value() { return value_; }

    /// Returns the underlying object.
    inline const Type& value() const { return value_; }

    /// The underlying value.
    Type value_;
};

template <internal::StringLiteral _name, class _Type>
inline auto make_field(const _Type& _value) {
    return Field<_name, _Type>(_value);
}

}  // namespace rfl

#endif  // RFL_FIELD_HPP_
