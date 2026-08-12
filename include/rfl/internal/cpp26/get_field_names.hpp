#ifndef RFL_INTERNAL_CPP26_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_CPP26_GETFIELDNAMES_HPP_

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

namespace rfl::internal::cpp26 {

template <class T>
auto get_field_names();

template <class T, class FieldName>
auto get_field_names_impl() {
  using Type = std::remove_cvref_t<T>;
  if constexpr (is_rename_v<Type>) {
    using Name = typename Type::Name;
    return Name();
  } else if constexpr (is_flatten_field_v<Type>) {
    return get_field_names<std::remove_cvref_t<typename Type::Type>>();
  } else {
    return FieldName();
  }
}

template <class T>
auto get_field_names() {
  using Type = std::remove_cvref_t<T>;
  if constexpr (std::is_pointer_v<Type>) {
    return get_field_names<std::remove_pointer_t<Type>>();
  } else {
    constexpr auto members =
        std::define_static_array(std::meta::nonstatic_data_members_of(
            ^^Type, std::meta::access_context::current()));

    using TupleT =
        std::invoke_result_t<decltype(bind_to_tuple(std::declval<Type&>()))>;

    static_assert(members.size() == tuple_size_v<TupleT>,
                  "Number of members and tuple size mismatch");

    constexpr bool no_flatten_fields = []<size_t... _is>(
                                           std::index_sequence<_is...>) {
      return (true && ... && !is_flatten_field_v<tuple_element_t<_is, TupleT>>);
    }(std::make_index_sequence<members.size()>{});

    if constexpr (no_flatten_fields) {
      return [&]<size_t... _is>(std::index_sequence<_is...>) {
        return Literal<
            StringLiteral<std::meta::identifier_of(members[_is]).size() + 1>(
                std::meta::identifier_of(members[_is]))...>();
      }(std::make_index_sequence<members.size()>());

    } else {
      return [&]<size_t... _is>(std::index_sequence<_is...>) {
        return concat_literals(
            get_field_names_impl<
                tuple_element_t<_is, TupleT>,
                Literal<StringLiteral<
                    std::meta::identifier_of(members[_is]).size() + 1>(
                    std::meta::identifier_of(members[_is]))>>()...);
      }(std::make_index_sequence<members.size()>());
    }
  }
}

}  // namespace rfl::internal::cpp26

#endif
