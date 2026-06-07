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
#include "../internal/allow_raw_ptrs_v.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class T>
struct is_span : std::false_type {};

template <class T, size_t Extent>
struct is_span<std::span<T, Extent>> : std::true_type {};

template <class T>
constexpr bool is_span_v = is_span<std::remove_cvref_t<T>>::value;

template <class R, class W, class T, class ProcessorsType>
struct ParserSpan {
  using InputVarType = typename R::InputVarType;
  using ParentType = Parent<W>;

  /**
   * @brief Reads a span from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed span or an error.
   */
  static Result<std::span<T>> read(const R& _r,
                                   const InputVarType& _var) noexcept {
    if constexpr (!internal::allow_raw_ptrs_v<ProcessorsType>) {
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

  /**
   * @brief Writes a span to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _span The span to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const std::span<T>& _span, const P& _parent) {
    auto arr = ParentType::add_array(_w, _span.size(), _parent);
    const auto new_parent = typename ParentType::Array{&arr};
    for (const auto& v : _span) {
      Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, v,
                                                                  new_parent);
    }
    _w.end_array(&arr);
  }

  /**
   * @brief Generates the schema for the span.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::vector<std::remove_cvref_t<T>>,
                  ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace rfl::parsing

#endif
