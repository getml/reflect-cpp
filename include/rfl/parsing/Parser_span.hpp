#ifndef RFL_PARSING_PARSER_SPAN_HPP_
#define RFL_PARSING_PARSER_SPAN_HPP_

#include <cstring>
#include <map>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::span<T>>
struct Parser<R, W, std::span<T>, ProcessorsType> {
  using InputVarType = typename R::InputVarType;
  using ParentType = Parent<W>;

  static Result<std::span<T>> read(const R& _r,
                                   const InputVarType& _var) noexcept {
    if constexpr (!ProcessorsType::allow_raw_ptrs_) {
      static_assert(always_false_v<R>,
                    "Reading into std::span is dangerous and "
                    "therefore unsupported. "
                    "Please consider using std::vector instead, or use the "
                    "rfl::AllowRawPtrs processor.");
      return error("Unsupported.");
    } else {
      return Parser<R, W, std::vector<std::remove_cvref_t<T>>,
                    ProcessorsType>::read(_r, _var)
          .transform([](std::vector<T>&& _vec) {
            using Type = std::remove_cvref_t<T>;
            Type* data = new Type[_vec.size()];
            for (size_t i = 0; i < _vec.size(); ++i) {
              data[i] = std::move(_vec[i]);
            }
            return std::span<T>(data, data + _vec.size());
          });
    }
  }

  template <class P>
  static void write(const W& _w, const std::span<T>& _span,
                    const P& _parent) noexcept {
    auto arr = ParentType::add_array(
        _w, std::distance(_span.begin(), _span.end()), _parent);
    const auto new_parent = typename ParentType::Array{&arr};
    for (const auto& v : _span) {
      Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, v,
                                                                  new_parent);
    }
    _w.end_array(&arr);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::vector<std::remove_cvref_t<T>>,
                  ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace rfl::parsing

#endif
