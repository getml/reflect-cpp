#ifndef RFL_PARSING_PARSER_RFL_ARRAY_HPP_
#define RFL_PARSING_PARSER_RFL_ARRAY_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../internal/Array.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_rfl_array : std::false_type {};

template <class T>
struct is_rfl_array<internal::Array<T>> : std::true_type {};

template <class T>
constexpr bool is_rfl_array_v = is_rfl_array<std::remove_cvref_t<T>>::value;

template <class R, class W, class ArrType, class ProcessorsType>
struct ParserRflArray {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /**
   * @brief Reads an rfl::internal::Array from the input.
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed std::array or an error.
   */
  static Result<ArrType> read(const R& _r, const InputVarType& _var) noexcept {
    return Parser<R, W, typename ArrType::StdArrayType, ProcessorsType>::read(
               _r, _var)
        .transform(
            [](auto&& _std_arr) { return ArrType(std::move(_std_arr)); });
  }

  /**
   * @brief Writes an rfl::internal::Array to the output.
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _arr The array to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const ArrType& _arr, const P& _parent) {
    Parser<R, W, typename ArrType::StdArrayType, ProcessorsType>::write(
        _w, _arr.arr_, _parent);
  }

  /**
   * @brief Generates the schema for rfl::internal::Array.
   * @param _definitions The map of definitions to add to.
   * @return The schema type for std::array.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, typename ArrType::StdArrayType,
                  ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace rfl::parsing

#endif
