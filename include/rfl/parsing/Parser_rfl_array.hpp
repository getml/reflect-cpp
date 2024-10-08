#ifndef RFL_PARSING_PARSER_RFL_ARRAY_HPP_
#define RFL_PARSING_PARSER_RFL_ARRAY_HPP_

#include <map>
#include <type_traits>

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

  static Result<internal::Array<T>> read(const R& _r,
                                         const InputVarType& _var) noexcept {
    return Parser<R, W, StdArray, ProcessorsType>::read(_r, _var);
  }

  template <class P>
  static void write(const W& _w, const internal::Array<T>& _arr,
                    const P& _parent) noexcept {
    Parser<R, W, StdArray, ProcessorsType>::write(_w, _arr.arr_, _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, StdArray, ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
