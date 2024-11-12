#ifndef RFL_PARSING_VARIANTALTERNATIVEWRAPPER_HPP_
#define RFL_PARSING_VARIANTALTERNATIVEWRAPPER_HPP_

#include <type_traits>

#include "../Field.hpp"
#include "../Literal.hpp"
#include "../internal/StringLiteral.hpp"
#include "../internal/get_type_name.hpp"
#include "../internal/has_tag_v.hpp"
#include "../internal/remove_namespaces.hpp"

namespace rfl::parsing {

namespace vaw {

template <class T>
struct GetName {};

template <internal::StringLiteral _name>
struct GetName<Literal<_name>> {
  constexpr static internal::StringLiteral name_ = _name;
};

template <class T>
consteval auto make_tag() {
  if constexpr (internal::has_tag_v<T>) {
    return typename T::Tag();
  } else {
    return Literal<
        internal::remove_namespaces<internal::get_type_name<T>()>()>();
  }
}

template <class T>
using tag_t = std::invoke_result_t<
    decltype(make_tag<std::remove_cvref_t<std::remove_pointer_t<T>>>)>;

}  // namespace vaw

template <class T>
using VariantAlternativeWrapper = Field<vaw::GetName<vaw::tag_t<T>>::name_, T>;

}  // namespace rfl::parsing

#endif
