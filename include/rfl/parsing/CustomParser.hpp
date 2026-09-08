#ifndef RFL_PARSING_CUSTOMPARSER_HPP_
#define RFL_PARSING_CUSTOMPARSER_HPP_

#include <exception>

#include "../Tuple.hpp"
#include "../internal/has_to_class_method_v.hpp"
#include "../internal/to_ptr_field_tuple.hpp"
#include "Parser.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class ProcessorsType, class OriginalClass,
          class HelperStruct>
struct CustomParser {
  using CustomParserHelperStruct = std::remove_cvref_t<HelperStruct>;

  /**
   * @brief Reads a value from the input.
   *
   * @param _r The reader to use.
   * @param _var The input variable to read.
   * @return A Result containing the parsed value or an error.
   */
  static Result<OriginalClass> read(const R& _r, const auto& _var) noexcept {
    const auto to_class = [](auto&& _h) -> Result<OriginalClass> {
      try {
        if constexpr (internal::has_to_class_method_v<HelperStruct>) {
          return _h.to_class();
        } else {
          auto ptr_field_tuple = internal::to_ptr_field_tuple(_h);
          const auto class_from_ptrs = [](auto&... _ptrs) {
            return OriginalClass(std::move(*_ptrs.value_)...);
          };
          return rfl::apply(class_from_ptrs, ptr_field_tuple);
        }
      } catch (std::exception& e) {
        return error(e.what());
      }
    };
    return Parser<R, W, CustomParserHelperStruct, ProcessorsType>::read(_r,
                                                                        _var)
        .and_then(to_class);
  }

  /**
   * @brief Writes a value to the output.
   *
   * @tparam P The type of the parent.
   * @param _w The writer to use.
   * @param _p The object to write.
   * @param _parent The parent object.
   */
  template <class P>
  static auto write(const W& _w, const OriginalClass& _p, const P& _parent) {
    Parser<R, W, CustomParserHelperStruct, ProcessorsType>::write(
        _w, HelperStruct::from_class(_p), _parent);
  }

  /**
   * @brief Generates the schema for the type.
   *
   * @param _definitions The map of definitions to add to.
   * @return The schema type.
   */
  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, CustomParserHelperStruct, ProcessorsType>::to_schema(
        _definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
