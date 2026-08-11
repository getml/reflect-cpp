#ifndef RFL_INTERNAL_CPP26_NUM_FIELDS_HPP_
#define RFL_INTERNAL_CPP26_NUM_FIELDS_HPP_

#include "../../Tuple.hpp"
#include "bind_to_tuple.hpp"

namespace rfl::internal::cpp26 {

template <class T>
constexpr std::size_t num_fields =
    tuple_size_v<decltype(bind_to_tuple(std::declval<T&>()))>;

}  // namespace rfl::internal::cpp26

#endif
