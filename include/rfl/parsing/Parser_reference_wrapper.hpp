#ifndef RFL_PARSING_PARSER_REFERENCE_WRAPPER_HPP_
#define RFL_PARSING_PARSER_REFERENCE_WRAPPER_HPP_

#include <functional>
#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::reference_wrapper<T>>
struct Parser<R, W, std::reference_wrapper<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  static Result<std::reference_wrapper<T>> read(const R&,
                                                const InputVarType&) noexcept {
    static_assert(always_false_v<T>,
                  "Reading into std::reference_wrapper is dangerous and "
                  "therefore unsupported. "
                  "Please consider using std::unique_ptr, rfl::Box, "
                  "std::shared_ptr, or"
                  "rfl::Ref instead.");
    return error("Unsupported.");
  }

  template <class P>
  static void write(const W& _w, const std::reference_wrapper<T> _ref,
                    const P& _p) noexcept {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, _ref.get(),
                                                                _p);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
