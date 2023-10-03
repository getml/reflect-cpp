#ifndef RFL_PARSING_CUSTOMPARSER_HPP_
#define RFL_PARSING_CUSTOMPARSER_HPP_

#include <exception>

#include "rfl/parsing/Parser.hpp"

namespace rfl {
namespace parsing {

template <class ReaderType, class WriterType, class OriginalClass,
          class HelperStruct>
struct CustomParser {
    static Result<OriginalClass> read(const ReaderType& _r,
                                      auto* _var) noexcept {
        const auto to_class = [](const auto& _h) -> Result<OriginalClass> {
            try {
                return _h.to_class();
            } catch (std::exception& e) {
                return Error(e.what());
            }
        };
        return Parser<ReaderType, WriterType, HelperStruct>::read(_r, _var)
            .and_then(to_class);
    }

    static auto write(const WriterType& _w, const OriginalClass& _p) noexcept {
        return Parser<ReaderType, WriterType, HelperStruct>::write(
            _w, HelperStruct::from_class(_p));
    }
};

}  // namespace parsing
}  // namespace rfl

#endif
