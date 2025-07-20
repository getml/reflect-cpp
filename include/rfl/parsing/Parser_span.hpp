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
      static_assert(
          always_false_v<R>,
          "Reading into std::span is dangerous and "
          "therefore unsupported. "
          "Please consider using std::vector instead or wrapping "
          "std::vector in rfl::Box or rfl::Ref."
          "If you absolutely must use std::span, "
          "you can pass the rfl::AllowRawPtrs processor. "
          "Please note that it is then YOUR responsibility "
          "to delete the allocated memory. Please also refer "
          "to the related documentation (in the section on processors).");
      return error("Unsupported.");
    } else {
      return Parser<R, W, std::vector<std::remove_cvref_t<T>>,
                    ProcessorsType>::read(_r, _var)
          .and_then([](std::vector<T>&& _vec) -> Result<std::span<T>> {
            using Type = std::remove_cvref_t<T>;
            Type* data = new (std::nothrow) Type[_vec.size()];
            if (!data) {
              return error("Failed to allocate memory for std::span.");
            }
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
    auto arr = ParentType::add_array(_w, _span.size(), _parent);
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
