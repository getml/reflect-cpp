#ifndef RFL_INTERNAL_FLATTEN_FIELD_TUPLE_HPP_
#define RFL_INTERNAL_FLATTEN_FIELD_TUPLE_HPP_

#include <tuple>

#include "rfl/internal/copy_to_field_tuple.hpp"
#include "rfl/internal/is_flatten_field.hpp"

namespace rfl {
namespace internal {

template <class FieldTuple, class... Args>
auto flatten_field_tuple(FieldTuple&& _t, Args&&... _args) {
    constexpr auto i = sizeof...(Args);
    if constexpr (i == std::tuple_size_v<std::decay_t<FieldTuple>>) {
        return std::tuple_cat(std::forward<Args>(_args)...);
    } else {
        using T = std::tuple_element_t<i, std::decay_t<FieldTuple>>;
        if constexpr (is_flatten_field<T>::value) {
            return flatten_field_tuple(
                std::forward<FieldTuple>(_t), std::forward<Args>(_args)...,
                flatten_field_tuple(copy_to_field_tuple(  // TODO: move_to_...
                    std::get<i>(_t).get())));
        } else {
            using T =
                std::decay_t<std::tuple_element_t<i, std::decay_t<FieldTuple>>>;
            return flatten_field_tuple(
                std::forward<FieldTuple>(_t), std::forward<Args>(_args)...,
                std::make_tuple(std::forward<T>(std::get<i>(_t))));
        }
    }
}

}  // namespace internal
}  // namespace rfl

#endif
