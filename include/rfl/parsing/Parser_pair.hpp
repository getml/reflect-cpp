#ifndef RFL_PARSING_PARSER_PAIR_HPP_
#define RFL_PARSING_PARSER_PAIR_HPP_

#include <map>
#include <type_traits>
#include <utility>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class FirstType, class SecondType,
          class ProcessorsType>
requires AreReaderAndWriter<R, W, std::pair<FirstType, SecondType>>
struct Parser<R, W, std::pair<FirstType, SecondType>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  /// Expresses the variables as type T.
  static Result<std::pair<FirstType, SecondType>> read(
      const R& _r, const InputVarType& _var) noexcept {
    const auto to_pair = [&](auto&& _t) {
      return std::make_pair(std::move(std::get<0>(_t)),
                            std::move(std::get<1>(_t)));
    };
    return Parser<R, W, std::tuple<FirstType, SecondType>,
                  ProcessorsType>::read(_r, _var)
        .transform(to_pair);
  }

  template <class P>
  static void write(const W& _w, const std::pair<FirstType, SecondType>& _p,
                    const P& _parent) noexcept {
    const auto tup = std::make_tuple(&_p.first, &_p.second);
    Parser<R, W, std::tuple<const FirstType*, const SecondType*>,
           ProcessorsType>::write(_w, tup, _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::tuple<FirstType, SecondType>,
                  ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
