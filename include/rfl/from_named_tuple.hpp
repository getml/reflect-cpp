#ifndef RFL_FROM_NAMED_TUPLE_HPP_
#define RFL_FROM_NAMED_TUPLE_HPP_

#include <functional>
#include <type_traits>

#include "rfl/internal/field_tuple_t.hpp"
#include "rfl/internal/has_flatten_fields.hpp"
#include "rfl/internal/is_named_tuple.hpp"
#include "rfl/named_tuple_t.hpp"

namespace rfl {

template <class FieldTupleType, class NamedTupleType, class... Args>
FieldTupleType concat_named_tuple(const NamedTupleType& _n, Args&&... _args);

/// Creates a struct of type T from a named tuple.
/// All fields of the struct must be an rfl::Field.
template <class T, class NamedTupleType>
T from_named_tuple(NamedTupleType&& _n) {
    using RequiredType = std::decay_t<named_tuple_t<T>>;

    if constexpr (internal::is_named_tuple_v<std::decay_t<T>>) {
        return std::forward<NamedTupleType>(_n);
    } else if constexpr (std::is_same<std::decay_t<NamedTupleType>,
                                      RequiredType>()) {
        const auto make = []<class... FieldTypes>(FieldTypes&&... _fields) {
            return T{std::forward<FieldTypes>(_fields)...};
        };

        using FieldTupleType = internal::field_tuple_t<std::decay_t<T>>;

        if constexpr (!internal::has_flatten_fields<FieldTupleType>()) {
            return std::apply(make, _n.fields());
        } else {
            const auto fields = concat_named_tuple<FieldTupleType>(_n);
            return std::apply(make, fields);
        }

    } else {
        return from_named_tuple<T, RequiredType>(
            RequiredType(std::forward<NamedTupleType>(_n)));
    }
}

/// Creates a struct of type T from a named tuple.
/// All fields of the struct must be an rfl::Field.
template <class T, class NamedTupleType>
T from_named_tuple(const NamedTupleType& _n) {
    using RequiredType = std::decay_t<named_tuple_t<T>>;
    if constexpr (internal::is_named_tuple_v<std::decay_t<T>>) {
        return _n;

    } else if constexpr (std::is_same<std::decay_t<NamedTupleType>,
                                      RequiredType>()) {
        const auto make = [](const auto&... _fields) { return T{_fields...}; };

        using FieldTupleType = internal::field_tuple_t<T>;

        if constexpr (!internal::has_flatten_fields<FieldTupleType>()) {
            return std::apply(make, _n.fields());
        } else {
            const auto fields = concat_named_tuple<FieldTupleType>(_n);
            return std::apply(make, fields);
        }

    } else {
        return from_named_tuple<T, RequiredType>(RequiredType(_n));
    }
}

template <class FieldTupleType, class NamedTupleType, class... Args>
FieldTupleType concat_named_tuple(const NamedTupleType& _n, Args&&... _args) {
    constexpr auto i = sizeof...(Args);
    if constexpr (i == std::tuple_size_v<std::decay_t<FieldTupleType>>) {
        return std::make_tuple(std::forward<Args>(_args)...);
    } else {
        using T = std::tuple_element_t<i, FieldTupleType>;
        if constexpr (internal::is_flatten_field<T>::value) {
            return concat_named_tuple<FieldTupleType>(
                _n, std::forward<Args>(_args)...,
                from_named_tuple<typename T::Type>(_n));
        } else {
            return concat_named_tuple<FieldTupleType>(
                _n, std::forward<Args>(_args)...,
                _n.template get_field<T::name_>());
        }
    }
}

}  // namespace rfl

#endif
