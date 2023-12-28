#ifndef RFL_PARSING_CUSTOMPARSER_HPP_
#define RFL_PARSING_CUSTOMPARSER_HPP_

#include <exception>
#include <tuple>

#include "../internal/has_to_class_method_v.hpp"
#include "../internal/to_ptr_field_tuple.hpp"
#include "Parser.hpp"

namespace rfl {
namespace parsing {

template <class ReaderType, class WriterType, class OriginalClass,
          class HelperStruct>
struct CustomParser {
  static Result<OriginalClass> read(const ReaderType& _r,
                                    const auto& _var) noexcept {
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
    return Parser<ReaderType, WriterType, HelperStruct>::read(_r, _var)
        .and_then(to_class);
  }

  template <class P>
  static auto write(const WriterType& _w, const OriginalClass& _p,
                    const P& _parent) noexcept {
    Parser<ReaderType, WriterType, HelperStruct>::write(
        _w, HelperStruct::from_class(_p), _parent);
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
