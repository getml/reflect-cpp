#ifndef RFL_PARSING_PARSER_PAIR_HPP_
#define RFL_PARSING_PARSER_PAIR_HPP_

#include <type_traits>
#include <utility>

#include "rfl/Ref.hpp"
#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/parsing/Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class FirstType, class SecondType>
requires AreReaderAndWriter<R, W, std::pair<FirstType, SecondType>>
struct Parser<R, W, std::pair<FirstType, SecondType>> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  /// Expresses the variables as type T.
  static Result<std::pair<FirstType, SecondType>> read(
      const R& _r, const InputVarType& _var) noexcept {
    const auto to_pair = [&](auto&& _t) {
      return std::make_pair(std::move(std::get<0>(_t)),
                            std::move(std::get<1>(_t)));
    };
    return Parser<R, W, std::tuple<FirstType, SecondType>>::read(_r, _var)
        .transform(to_pair);
  }

  /// Transform a std::pair into an array
  static OutputVarType write(
      const W& _w, const std::pair<FirstType, SecondType>& _p) noexcept {
    const auto tup = std::make_tuple(&_p.first, &_p.second);
    return Parser<R, W, std::tuple<const FirstType*, const SecondType*>>::write(
        _w, tup);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
