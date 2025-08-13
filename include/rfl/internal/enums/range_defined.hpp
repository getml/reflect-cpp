#ifndef RFL_INTERNAL_ENUMS_RANGE_DEFINED_HPP_
#define RFL_INTERNAL_ENUMS_RANGE_DEFINED_HPP_

#include <concepts>

#include "../../config.hpp"

namespace rfl::internal::enums {

template <class E>
concept range_defined = requires {
  { config::enum_range<E>::min };

  { config::enum_range<E>::max };
};

}  // namespace rfl::internal::enums

#endif
