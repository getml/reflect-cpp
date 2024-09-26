#ifndef RFL_INTERNAL_GETMETAFIELDS_HPP_
#define RFL_INTERNAL_GETMETAFIELDS_HPP_

#include <array>
#include <tuple>
#include <utility>

#include "../MetaField.hpp"
#include "../Tuple.hpp"
#include "../type_name_t.hpp"

namespace rfl {
namespace internal {

template <class NamedTupleType, class... AlreadyExtracted>
auto get_meta_fields(AlreadyExtracted&&... _already_extracted) {
  constexpr size_t i = sizeof...(_already_extracted);
  if constexpr (NamedTupleType::size() == i) {
    return std::array<MetaField, i>{std::move(_already_extracted)...};
  } else {
    using FieldType = tuple_element_t<i, typename NamedTupleType::Fields>;
    auto name = typename FieldType::Name().str();
    auto type = type_name_t<typename FieldType::Type>().str();
    return get_meta_fields<NamedTupleType>(
        std::move(_already_extracted)...,
        MetaField(std::move(name), std::move(type)));
  }
}

}  // namespace internal
}  // namespace rfl

#endif
