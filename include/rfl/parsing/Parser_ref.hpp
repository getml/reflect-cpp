#ifndef RFL_PARSING_PARSER_REF_HPP_
#define RFL_PARSING_PARSER_REF_HPP_

#include <map>
#include <type_traits>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../atomic/is_atomic.hpp"
#include "../atomic/remove_atomic_t.hpp"
#include "../atomic/set_atomic.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, Ref<T>>
struct Parser<R, W, Ref<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  static Result<Ref<T>> read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (atomic::is_atomic_v<T>) {
      using RemoveAtomicT = atomic::remove_atomic_t<T>;
      return Parser<R, W, RemoveAtomicT, ProcessorsType>::read(_r, _var)
          .transform([](auto&& _t) {
            auto atomic_ref = Ref<T>::make();
            atomic::set_atomic(std::move(_t), &(*atomic_ref));
            return atomic_ref;
          });

    } else {
      const auto to_ref = [&](auto&& _t) {
        return Ref<T>::make(std::move(_t));
      };
      return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::read(_r,
                                                                        _var)
          .transform(to_ref);
    }
  }

  template <class P>
  static void write(const W& _w, const Ref<T>& _ref, const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_ref,
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
