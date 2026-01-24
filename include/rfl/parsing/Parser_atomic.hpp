#ifndef RFL_PARSING_PARSER_ATOMIC_HPP_
#define RFL_PARSING_PARSER_ATOMIC_HPP_

#include <atomic>
#include <map>
#include <type_traits>

#include "../DefaultVal.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::atomic<T>>
struct Parser<R, W, std::atomic<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  /// Read is not supported for atomic types - we must used rfl::atomic instead.

  template <class P>
  static void write(const W& _w, const std::atomic<T>& _a, const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
        _w, _a.load(std::memory_order_relaxed), _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    return schema::Type{
        Parser<R, W, U, ProcessorsType>::to_schema(_definitions)};
  }
};

}  // namespace rfl::parsing

#endif
