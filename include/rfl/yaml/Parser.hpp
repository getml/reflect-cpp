#ifndef RFL_YAML_PARSER_HPP_
#define RFL_YAML_PARSER_HPP_

#include "../parsing/Parser.hpp"
#include "Reader.hpp"
#include "Writer.hpp"

namespace rfl {
namespace yaml {

template <class T>
using Parser = parsing::Parser<Reader, Writer, T>;

}
}  // namespace rfl

#endif
