#ifndef RFL_PARSING_PARSER_ATOMIC_FLAG_HPP_
#define RFL_PARSING_PARSER_ATOMIC_FLAG_HPP_

#include <atomic>
#include <map>
#include <type_traits>

#include "../DefaultVal.hpp"
#include "AreReaderAndWriter.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::atomic_flag>
struct Parser<R, W, std::atomic_flag, ProcessorsType> {
  using InputVarType = typename R::InputVarType;

  /// Read is not supported for atomic types - we must used rfl::atomic instead.

  template <class P>
  static void write(const W& _w, const std::atomic_flag& _a, const P& _parent) {
    Parser<R, W, bool, ProcessorsType>::write(
        _w, _a.test(std::memory_order_relaxed), _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return schema::Type{
        Parser<R, W, bool, ProcessorsType>::to_schema(_definitions)};
  }
};

}  // namespace rfl::parsing

#endif
