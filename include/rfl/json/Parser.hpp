#ifndef RFL_JSON_PARSER_HPP_
#define RFL_JSON_PARSER_HPP_

#include "rfl/json/Reader.hpp"
#include "rfl/json/Writer.hpp"
#include "rfl/parsing/Parser.hpp"

namespace rfl {
namespace json {

template <class T>
using Parser = parsing::Parser<Reader, Writer, T>;

}
}  // namespace rfl

#endif
