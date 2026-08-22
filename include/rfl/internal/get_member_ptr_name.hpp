#ifndef RFL_INTERNAL_GET_MEMBER_PTR_NAME_HPP_
#define RFL_INTERNAL_GET_MEMBER_PTR_NAME_HPP_

#include "cpp20/get_member_ptr_name.hpp"

namespace rfl::internal {

template <auto ptr>
consteval auto get_member_ptr_name() {
  return cpp20::get_member_ptr_name<ptr>();
}

}  // namespace rfl::internal

#endif
