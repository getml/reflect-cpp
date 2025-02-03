#ifndef RFL_PARSING_PARSER_PTR_HPP_
#define RFL_PARSING_PARSER_PTR_HPP_

#include <map>
#include <type_traits>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, T*>
struct Parser<R, W, T*, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /// Expresses the variables as type T.
  static Result<T*> read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (!ProcessorsType::allow_raw_ptrs_) {
      static_assert(
          always_false_v<T>,
          "Reading into raw pointers is dangerous and "
          "therefore unsupported by default. "
          "Please consider using std::unique_ptr, rfl::Box, "
          "std::shared_ptr, "
          "rfl::Ref or std::optional instead. "
          "If you absolutely must use raw pointers, "
          "you can pass the rfl::AllowRawPtrs processor. "
          "Please note that it is then YOUR responsibility "
          "to delete the allocated memory. Please also refer "
          "to the related documentation (in the section on processors).");
      return error("Unsupported.");
    } else {
      if (_r.is_empty(_var)) {
        return nullptr;
      }
      return Parser<R, W, T, ProcessorsType>::read(_r, _var).transform(
          [](T&& _t) { return new T(std::move(_t)); });
    }
  }

  template <class P>
  static void write(const W& _w, const T* _ptr, const P& _parent) noexcept {
    if (!_ptr) {
      ParentType::add_null(_w, _parent);
      return;
    }
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_ptr,
                                                                _parent);
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
