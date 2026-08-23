#ifndef RFL_INTERNAL_ENUMS_CPP26_TO_STRING_HPP_
#define RFL_INTERNAL_ENUMS_CPP26_TO_STRING_HPP_

#ifdef REFLECTCPP_USE_CPP26_REFLECTION

#include <meta>
#include <string>
#include <type_traits>

namespace rfl::internal::enums::cpp26 {

template <class EnumType>
  requires std::is_enum_v<EnumType>
std::string to_string(const EnumType _val) {
  template for (constexpr auto e : std::define_static_array(
                    std::meta::enumerators_of(^^EnumType))) {
    if (_val == [:e:]) {
      return std::string(std::meta::identifier_of(e));
    }
  }
  return std::to_string(static_cast<std::underlying_type_t<EnumType>>(_val));
}

}  // namespace rfl::internal::enums::cpp26

#endif
#endif
