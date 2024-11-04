#ifndef RFL_PARSING_VARIANTALTERNATIVEWRAPPER_HPP_
#define RFL_PARSING_VARIANTALTERNATIVEWRAPPER_HPP_

#include <type_traits>

#include "../Field.hpp"
#include "../internal/get_type_name.hpp"
#include "../internal/remove_namespaces.hpp"

namespace rfl::parsing {

template <class T>
using VariantAlternativeWrapper =
    Field<internal::remove_namespaces<internal::get_type_name<
              std::remove_cvref_t<std::remove_pointer_t<T>>>()>(),
          T>;

}  // namespace rfl::parsing

#endif
