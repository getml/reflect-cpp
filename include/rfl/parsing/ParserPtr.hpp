#ifndef RFL_PARSING_PARSER_PTR_HPP_
#define RFL_PARSING_PARSER_PTR_HPP_

#include <map>
#include <type_traits>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/allow_raw_ptrs_v.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "schema/Type.hpp"

namespace rfl::parsing {

template <class R, class W, class T, class ProcessorsType>
struct ParserPtr {
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  /**
   * @brief Reads a value into a raw pointer from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read from.
   * @return A Result containing the parsed pointer or an error.
   */
  static Result<T*> read(const R& _r, const InputVarType& _var) noexcept {
    if constexpr (!internal::allow_raw_ptrs_v<ProcessorsType>) {
      static_assert(
          always_false_v<T>,
          "Reading into raw pointers is dangerous and "
          "therefore unsupported by default. "
          "Please consider using std::unique_ptr, rfl::Box, "
          "std::shared_ptr, "
          "rfl::Ref or std::optional instead. "
          "If you absolutely must use raw pointers, "
          "you can pass the rfl::AllowRawPtrs processor. "
          "Please note that it is then YOUR responsibility "
          "to delete the allocated memory. Please also refer "
          "to the related documentation (in the section on processors).");
      return error("Unsupported.");
    } else {
      if (_r.is_empty(_var)) {
        return nullptr;
      }
      return Parser<R, W, T, ProcessorsType>::read(_r, _var).transform(
          [](T&& _t) { return new T(std::move(_t)); });
    }
  }

  /**
   * @brief Writes a value from a raw pointer to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _ptr The pointer to write.
   * @param _parent The parent object.
   */
  template <class P>
  static void write(const W& _w, const T* _ptr, const P& _parent) {
    if (!_ptr) {
      ParentType::add_null(_w, _parent);
      return;
    }
    Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, *_ptr,
                                                                _parent);
  }

  /**
   * @brief Generates the schema for the pointer.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace rfl::parsing

#endif
