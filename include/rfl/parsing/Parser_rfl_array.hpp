#ifndef RFL_PARSING_PARSER_RFL_ARRAY_HPP_
#define RFL_PARSING_PARSER_RFL_ARRAY_HPP_

#include <map>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/Array.hpp"
#include "../internal/to_std_array.hpp"
#include "Parser_array.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, internal::Array<T>>
struct Parser<R, W, internal::Array<T>, ProcessorsType> {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using StdArray = internal::to_std_array_t<T>;

  /**
   * @brief Reads an rfl::Array from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed rfl::Array or an error.
   */
  static Result<internal::Array<T>> read(const R& _r,
                                         const InputVarType& _var) noexcept {
    return Parser<R, W, StdArray, ProcessorsType>::read(_r, _var);
  }

  /**
   * @brief Writes an rfl::Array to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _arr The rfl::Array to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const internal::Array<T>& _arr,
                    const P& _parent) {
    Parser<R, W, StdArray, ProcessorsType>::write(_w, _arr.arr_, _parent);
  }

  /**
   * @brief Generates the schema for the rfl::Array.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, StdArray, ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
