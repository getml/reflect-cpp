#ifndef RFL_PARSING_CUSTOMPARSER_HPP_
#define RFL_PARSING_CUSTOMPARSER_HPP_

#include <exception>
#include <tuple>

#include "../internal/has_to_class_method_v.hpp"
#include "../internal/to_ptr_field_tuple.hpp"
#include "Parser.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class ProcessorsType, class OriginalClass,
          class HelperStruct>
struct CustomParser {
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
          return std::apply(class_from_ptrs, ptr_field_tuple);
        }
      } catch (std::exception& e) {
        return Error(e.what());
      }
    };
    return Parser<R, W, HelperStruct, ProcessorsType>::read(_r, _var).and_then(
        to_class);
  }

  template <class P>
  static auto write(const W& _w, const OriginalClass& _p,
                    const P& _parent) noexcept {
    Parser<R, W, HelperStruct, ProcessorsType>::write(
        _w, HelperStruct::from_class(_p), _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<R, W, std::remove_cvref_t<HelperStruct>,
                  ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
