#ifndef RFL_INTERNAL_CPP26_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_CPP26_GETFIELDNAMES_HPP_

#ifdef REFLECTCPP_USE_CPP26_REFLECTION

#include <format>
#include <functional>
#include <meta>
#include <type_traits>
#include <utility>

#include "../../Literal.hpp"
#include "../../Tuple.hpp"
#include "../StringLiteral.hpp"
#include "../concat_literals.hpp"
#include "../is_flatten_field.hpp"
#include "../is_rename.hpp"
#include "bind_to_tuple.hpp"
#include "get_members.hpp"

namespace rfl::internal::cpp26 {

template <class T>
consteval auto get_field_names();

template <class T, class FieldName>
consteval auto get_field_names_impl() {
  using Type = std::remove_cvref_t<std::remove_pointer_t<T>>;
  if constexpr (is_rename_v<Type>) {
    using Name = typename Type::Name;
    return Name();
  } else if constexpr (is_flatten_field_v<Type>) {
    return get_field_names<std::remove_cvref_t<typename Type::Type>>();
  } else {
    return FieldName();
  }
}

template <auto _i, auto _j>
consteval bool is_same() {
  static_assert(_i == _j, "Mismatched lengths");
  return _i == _j;
}

template <class T>
consteval auto get_field_names() {
  using Type = std::remove_cvref_t<T>;
  if constexpr (std::is_pointer_v<Type>) {
    return get_field_names<std::remove_pointer_t<Type>>();
  } else {
    constexpr auto members = std::define_static_array(get_members<Type>());

    using TupleT = decltype(bind_to_tuple(std::declval<Type&>()));

    static_assert(is_same<members.size(), tuple_size_v<TupleT>>(),
                  "Number of members and tuple size mismatch");

    constexpr auto [... is] = std::make_index_sequence<members.size()>();

    return concat_literals(
        get_field_names_impl<
            tuple_element_t<is, TupleT>,
            Literal<
                StringLiteral<std::meta::identifier_of(members[is]).size() + 1>(
                    std::meta::identifier_of(members[is]))>>()...);
  }
}

}  // namespace rfl::internal::cpp26

#endif
#endif
