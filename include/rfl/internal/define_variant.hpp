#ifndef RFL_INTERNAL_DEFINEVARIANT_HPP_
#define RFL_INTERNAL_DEFINEVARIANT_HPP_

#include <variant>

namespace rfl {
namespace internal {

/// Allows you to combine several variants.
template <class... Vars>
struct define_variant;

/// Recursive case - both variants.
template <class... Vars1, class... Vars2, class... Tail>
struct define_variant<std::variant<Vars1...>, std::variant<Vars2...>, Tail...> {
    using type = typename define_variant<std::variant<Vars1..., Vars2...>,
                                         Tail...>::type;
};

/// Recursive case - variant plus other type.
template <class... Vars, class Head, class... Tail>
struct define_variant<std::variant<Vars...>, Head, Tail...> {
    using type =
        typename define_variant<std::variant<Vars..., Head>, Tail...>::type;
};

/// Recursive case - other type.
template <class Head, class... Tail>
struct define_variant<Head, Tail...> {
    using type = typename define_variant<std::variant<Head>, Tail...>::type;
};

/// Special case - only a single variant is left.
template <class... Vars>
struct define_variant<std::variant<Vars...>> {
    using type = std::variant<Vars...>;
};

}  // namespace internal
}  // namespace rfl

#endif
