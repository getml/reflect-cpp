#ifndef RFL_INTERNAL_REMOVE_NAMESPACES_HPP_
#define RFL_INTERNAL_REMOVE_NAMESPACES_HPP_

#include <string_view>
#include <utility>

#include "StringLiteral.hpp"

namespace rfl {
namespace internal {

template <StringLiteral _name>
consteval auto remove_namespaces() {
  constexpr auto name = _name.string_view();
  constexpr size_t pos = name.find_last_of(":");
  if constexpr (pos == std::string_view::npos) {
    return _name;
  }
  constexpr auto substr = name.substr(pos + 1);
  const auto to_str_lit = [&]<auto... Ns>(std::index_sequence<Ns...>) {
    return StringLiteral<sizeof...(Ns) + 1>{substr[Ns]...};
  };
  return to_str_lit(std::make_index_sequence<substr.size()>{});
}

}  // namespace internal
}  // namespace rfl

#endif
