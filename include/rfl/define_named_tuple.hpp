#ifndef RFL_DEFINENAMEDTUPLE_HPP_
#define RFL_DEFINENAMEDTUPLE_HPP_

#include "NamedTuple.hpp"
#include "internal/define_named_tuple.hpp"

namespace rfl {

template <class... FieldTypes>
using define_named_tuple_t =
    typename internal::define_named_tuple<FieldTypes...>::type;

}  // namespace rfl

#endif  // RFL_DEFINENAMEDTUPLE_HPP_
