#ifndef RFL_PARSING_PARSER_C_ARRAY_HPP_
#define RFL_PARSING_PARSER_C_ARRAY_HPP_

#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/to_std_array.hpp"
#include "Parser_array.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, size_t _size>
  requires AreReaderAndWriter<R, W, T[_size]>
struct Parser<R, W, T[_size]> {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using OutputArrayType = typename W::OutputArrayType;
  using OutputVarType = typename W::OutputVarType;

  using CArray = T[_size];
  using StdArray = internal::to_std_array_t<T[_size]>;

  static auto read(const R& _r, const InputVarType& _var) noexcept {
    return Parser<R, W, StdArray>::read(_r, _var);
  }

  static OutputVarType write(const W& _w, const CArray& _arr) noexcept {
    auto arr = _w.new_array();
    for (const auto& e : _arr) {
      _w.add(Parser<R, W, std::remove_cvref_t<T>>::write(_w, e), &arr);
    }
    return OutputVarType(arr);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
