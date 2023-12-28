#ifndef RFL_INTERNAL_DEFINENAMEDTUPLE_HPP_
#define RFL_INTERNAL_DEFINENAMEDTUPLE_HPP_

#include "../NamedTuple.hpp"

namespace rfl {
namespace internal {

template <class... FieldTypes>
struct define_named_tuple;

/// Allows you to combine several named tuples and/or additional fields.
/// Recursive case - all types are fields.
template <class Head, class... Tail>
struct define_named_tuple<Head, Tail...> {
    using type = typename define_named_tuple<NamedTuple<Head>, Tail...>::type;
};

/// Allows you to combine several named tuples and/or additional fields.
/// Recursive case - first type is NamedTuple, second type is field.
template <class Head, class... TupContent, class... Tail>
struct define_named_tuple<NamedTuple<TupContent...>, Head, Tail...> {
    using type = typename define_named_tuple<NamedTuple<TupContent..., Head>,
                                             Tail...>::type;
};

/// Allows you to combine several named tuples and/or additional fields.
/// Recursive case - first type is NamedTuple, second type is also NamedTuple.
template <class... TupContent, class... TupContent2, class... Tail>
struct define_named_tuple<NamedTuple<TupContent...>, NamedTuple<TupContent2...>,
                          Tail...> {
    using type =
        typename define_named_tuple<NamedTuple<TupContent..., TupContent2...>,
                                    Tail...>::type;
};

/// Allows you to combine several named tuples and/or additional fields.
template <class... TupContent>
struct define_named_tuple<NamedTuple<TupContent...>> {
    using type = NamedTuple<TupContent...>;
};

}  // namespace internal
}  // namespace rfl

#endif
