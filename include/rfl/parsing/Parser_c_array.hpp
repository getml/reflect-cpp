#ifndef RFL_PARSING_PARSER_C_ARRAY_HPP_
#define RFL_PARSING_PARSER_C_ARRAY_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/Array.hpp"
#include "../internal/to_std_array.hpp"
#include "Parent.hpp"
#include "Parser_array.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, size_t _size, class ProcessorsType>
requires AreReaderAndWriter<R, W, T[_size]>
struct Parser<R, W, T[_size], ProcessorsType> {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;
  using CArray = T[_size];

  static Result<internal::Array<CArray>> read(
      const R& _r, const InputVarType& _var) noexcept {
    using StdArray = internal::to_std_array_t<CArray>;
    return Parser<R, W, StdArray, ProcessorsType>::read(_r, _var);
  }

  template <class P>
  static void write(const W& _w, const CArray& _arr,
                    const P& _parent) noexcept {
    auto arr = ParentType::add_array(_w, _size, _parent);
    const auto new_parent = typename ParentType::Array{&arr};
    for (const auto& e : _arr) {
      Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, e,
                                                                  new_parent);
    }
    _w.end_array(&arr);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using StdArray = internal::to_std_array_t<CArray>;
    return Parser<R, W, std::remove_cvref_t<StdArray>,
                  ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
