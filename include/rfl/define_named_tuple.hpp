#ifndef RFL_DEFINENAMEDTUPLE_HPP_
#define RFL_DEFINENAMEDTUPLE_HPP_

#include "rfl/NamedTuple.hpp"
#include "rfl/internal/define_named_tuple.hpp"

namespace rfl {

template <class... FieldTypes>
using define_named_tuple_t =
    typename internal::define_named_tuple<FieldTypes...>::type;

}  // namespace rfl

#endif  // RFL_DEFINENAMEDTUPLE_HPP_
