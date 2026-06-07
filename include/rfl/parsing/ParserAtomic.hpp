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

template <class T>
struct is_atomic : std::false_type {};

template <class T>
struct is_atomic<std::atomic<T>> : std::true_type {};

template <class T>
constexpr bool is_atomic_v = is_atomic<std::remove_cvref_t<T>>::value;

template <class R, class W, class AtomicType, class ProcessorsType>
struct ParserAtomic {
  using T = typename std::remove_cvref_t<AtomicType>::value_type;

  using InputVarType = typename R::InputVarType;

  /**
   * @brief Writes an atomic value to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _a The atomic value to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::atomic<T>& _a, const P& _parent) {
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(
        _w, _a.load(std::memory_order_relaxed), _parent);
  }

  /**
   * @brief Generates the schema for the atomic value.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    return schema::Type{
        Parser<R, W, U, ProcessorsType>::to_schema(_definitions)};
  }
};

}  // namespace rfl::parsing

#endif
