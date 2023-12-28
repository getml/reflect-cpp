#ifndef RFL_INTERNAL_LIT_NAME_HPP_
#define RFL_INTERNAL_LIT_NAME_HPP_

#include "../Literal.hpp"

namespace rfl {
namespace internal {

template <class T>
struct lit_name;

template <auto _name>
struct lit_name<rfl::Literal<_name>> {
  constexpr static auto name_ = _name;
};

template <class LiteralType>
constexpr auto lit_name_v = lit_name<LiteralType>::name_;

}  // namespace internal
}  // namespace rfl

#endif
