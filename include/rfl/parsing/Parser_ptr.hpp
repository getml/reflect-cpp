#ifndef RFL_PARSING_PARSER_PTR_HPP_
#define RFL_PARSING_PARSER_PTR_HPP_

#include <type_traits>

#include "rfl/Result.hpp"
#include "rfl/always_false.hpp"
#include "rfl/parsing/Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, T*>
struct Parser<R, W, T*> {
  using InputVarType = typename R::InputVarType;
  using OutputVarType = typename W::OutputVarType;

  /// Expresses the variables as type T.
  static Result<T*> read(const R& _r, const InputVarType& _var) noexcept {
    static_assert(always_false_v<T>,
                  "Reading into raw pointers is dangerous and "
                  "therefore unsupported. "
                  "Please consider using std::unique_ptr, rfl::Box, "
                  "std::shared_ptr, "
                  "rfl::Ref or std::optional instead.");
    return Error("Unsupported.");
  }

  /// Expresses the variable a a JSON.
  static OutputVarType write(const W& _w, const T* _ptr) noexcept {
    if (!_ptr) {
      return _w.empty_var();
    }
    return Parser<R, W, std::decay_t<T>>::write(_w, *_ptr);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
