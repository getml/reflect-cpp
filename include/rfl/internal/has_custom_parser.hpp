#ifndef RFL_INTERNAL_HASCUSTOMPARSER_HPP_
#define RFL_INTERNAL_HASCUSTOMPARSER_HPP_

#include <type_traits>

#include "../parsing/Parser.hpp"

namespace rfl::internal {

template <class R, class W, class T, class ProcessorsType>
concept has_custom_parser = requires(
    const T& _t, const typename parsing::Parser<
                     R, W, T, ProcessorsType>::CustomParserHelperStruct& _h) {
  {
    std::remove_cvref_t<decltype(_h)>::from_class(_t)
  } -> std::same_as<std::remove_cvref_t<decltype(_h)>>;

  { _h.to_class() } -> std::same_as<T>;
};

}  // namespace rfl::internal

#endif
