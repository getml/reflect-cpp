#ifndef RFL_XML_PARSER_HPP_
#define RFL_XML_PARSER_HPP_

#include <type_traits>

#include "../internal/is_attribute.hpp"
#include "../parsing/NamedTupleParser.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace parsing {

/// XML is very special. It doesn't have proper support for arrays, which means
/// that we just need to ignore empty containers. Therefore, we need to a
/// template specialization for the NamedTuple parser to accommodate for it.
template <class ProcessorsType, class... FieldTypes>
requires AreReaderAndWriter<xml::Reader, xml::Writer, NamedTuple<FieldTypes...>>
struct Parser<xml::Reader, xml::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<xml::Reader, xml::Writer,
                              /*_ignore_empty_containers=*/true,
                              /*_all_required=*/ProcessorsType::all_required_,
                              /*_no_field_names_=*/false, ProcessorsType,
                              FieldTypes...> {
};

}  // namespace parsing
}  // namespace rfl

namespace rfl {
namespace xml {

template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}
}  // namespace rfl

#endif
