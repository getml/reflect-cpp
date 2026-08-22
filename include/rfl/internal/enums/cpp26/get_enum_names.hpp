#ifndef RFL_INTERNAL_ENUMS_CPP26_GET_ENUM_NAMES_HPP_
#define RFL_INTERNAL_ENUMS_CPP26_GET_ENUM_NAMES_HPP_

#ifdef REFLECTCPP_USE_CPP26_REFLECTION

#include <concepts>
#include <meta>
#include <utility>

#include "../../../Literal.hpp"
#include "../Names.hpp"
#include "../is_flag_enum.hpp"

namespace rfl::internal::enums::cpp26 {

template <class EnumType>
consteval auto get_enum_names() {
  constexpr auto enumerators =
      std::define_static_array(std::meta::enumerators_of(^^EnumType));

  constexpr auto [... is] = std::make_index_sequence<enumerators.size()>();

  return Names<
      EnumType,
      Literal<StringLiteral<std::meta::identifier_of(enumerators[is]).size() +
                            1>(std::meta::identifier_of(enumerators[is]))...>,
      enumerators.size(), is_flag_enum<EnumType>, ([:enumerators[is]:])...>{};
}
}  // namespace rfl::internal::enums::cpp26

#endif
#endif
