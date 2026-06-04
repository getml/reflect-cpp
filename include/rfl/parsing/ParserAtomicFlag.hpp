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

template <class T>
struct is_atomic_flag : std::false_type {};

template <>
struct is_atomic_flag<std::atomic_flag> : std::true_type {};

template <class T>
constexpr bool is_atomic_flag_v = is_atomic_flag<std::remove_cvref_t<T>>::value;

template <class R, class W, class ProcessorsType>
struct ParserAtomicFlag {
  using InputVarType = typename R::InputVarType;

  /**
   * @brief Writes an atomic_flag to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _a The atomic_flag to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::atomic_flag& _a, const P& _parent) {
    Parser<R, W, bool, ProcessorsType>::write(
        _w, _a.test(std::memory_order_relaxed), _parent);
  }

  /**
   * @brief Generates the schema for the atomic_flag.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return schema::Type{
        Parser<R, W, bool, ProcessorsType>::to_schema(_definitions)};
  }
};

}  // namespace rfl::parsing

#endif
