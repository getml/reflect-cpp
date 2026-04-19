#ifndef RFL_XML_PARSER_HPP_
#define RFL_XML_PARSER_HPP_

#include <type_traits>

#include "../Generic.hpp"
#include "../always_false.hpp"
#include "../internal/is_attribute.hpp"
#include "../internal/no_optionals_v.hpp"
#include "../parsing/NamedTupleParser.hpp"
#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl::parsing {

/// XML is very special. It doesn't have proper support for arrays, which means
/// that we just need to ignore empty containers. Therefore, we need to a
/// template specialization for the NamedTuple parser to accommodate for it.
template <class ProcessorsType, class... FieldTypes>
  requires AreReaderAndWriter<xml::Reader, xml::Writer,
                              NamedTuple<FieldTypes...>>
struct Parser<xml::Reader, xml::Writer, NamedTuple<FieldTypes...>,
              ProcessorsType>
    : public NamedTupleParser<
          xml::Reader, xml::Writer,
          /*_ignore_empty_containers=*/true,
          /*_all_required=*/internal::no_optionals_v<ProcessorsType>,
          /*_no_field_names_=*/false, ProcessorsType, FieldTypes...> {};

/// The generic parser is also special, because we need to ignore empty
/// containers, which means that we need to try to read the value as an array or
/// object before trying to read it as a string. Therefore, we need to a
/// template specialization for the generic parser to accommodate for it.
template <class ProcessorsType>
  requires AreReaderAndWriter<xml::Reader, xml::Writer, Generic>
struct Parser<xml::Reader, xml::Writer, Generic, ProcessorsType> {
  using InputVarType = typename xml::Reader::InputVarType;

  static Result<Generic> read(const xml::Reader& _r,
                              const InputVarType& _var) noexcept {
    static_assert(always_false_v<ProcessorsType>,
                  "XML does not support reading generic types.");
    return error("XML does not support reading generic types.");
  }

  static void write(const xml::Writer& _w, const Generic& _generic,
                    const auto& _parent) {
    Parser<xml::Reader, xml::Writer, Generic::ReflectionType,
           ProcessorsType>::write(_w, _generic.reflection(), _parent);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    return Parser<xml::Reader, xml::Writer, Generic::ReflectionType,
                  ProcessorsType>::to_schema(_definitions);
  }
};

}  // namespace rfl::parsing

namespace rfl::xml {
template <class T, class ProcessorsType>
using Parser = parsing::Parser<Reader, Writer, T, ProcessorsType>;

}  // namespace rfl::xml

#endif
