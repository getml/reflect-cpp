#ifndef RFL_INTERNAL_ENUMS_IS_FLAG_ENUM_HPP_
#define RFL_INTERNAL_ENUMS_IS_FLAG_ENUM_HPP_

#include <concepts>

namespace rfl::internal::enums {

// Checks if an enum is a flag enum.
template <class EnumType>
inline constexpr bool is_flag_enum = requires(EnumType e) {
  { e | e } -> std::same_as<EnumType>;
};

}  // namespace rfl::internal::enums

#endif
