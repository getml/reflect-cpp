#ifndef RFL_INTERNAL_CPP26_GETFIELDNAMES_HPP_
#define RFL_INTERNAL_CPP26_GETFIELDNAMES_HPP_

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

template <class Type>
consteval std::vector<std::meta::info> get_members() {
  constexpr auto ctx = std::meta::access_context::current();

  std::vector<std::meta::info> members;

  constexpr auto nonstatic_members = std::define_static_array(
      std::meta::nonstatic_data_members_of(^^Type, ctx));

  template for (auto member : nonstatic_members) { members.push_back(member); }

  return members;
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

}  // namespace rfl::internal::cpp26

#endif
