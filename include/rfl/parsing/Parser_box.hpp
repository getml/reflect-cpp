#ifndef RFL_PARSING_PARSER_BOX_HPP_
#define RFL_PARSING_PARSER_BOX_HPP_

#include <map>
#include <type_traits>

#include "../Box.hpp"
#include "../Result.hpp"
#include "../atomic/is_atomic.hpp"
#include "../atomic/remove_atomic_t.hpp"
#include "../atomic/set_atomic.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class T, Copyability C, class ProcessorsType>
  requires AreReaderAndWriter<R, W, Box<T, C>>
struct Parser<R, W, Box<T, C>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  static Result<Box<T, C>> read(const R& _r,
                                const InputVarType& _var) noexcept {
    if constexpr (atomic::is_atomic_v<T>) {
      using RemoveAtomicT = atomic::remove_atomic_t<T>;
      return Parser<R, W, RemoveAtomicT, ProcessorsType>::read(_r, _var)
          .transform([](auto&& _t) {
            auto atomic_box = Box<T>::make();
            atomic::set_atomic(std::move(_t), &(*atomic_box));
            return atomic_box;
          });

    } else {
      const auto to_box = [](auto&& _t) {
        return Box<T, C>::make(std::move(_t));
      };
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform(to_box);
    }
  }

  template <class P>
  static void write(const W& _w, const Box<T, C>& _box, const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_box,
                                                                _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace rfl::parsing

#endif
