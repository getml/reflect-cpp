#ifndef RFL_PARSING_SCHEMA_MAKE_HPP_
#define RFL_PARSING_SCHEMA_MAKE_HPP_

#include <map>

#include "../Parser.hpp"
#include "Definition.hpp"
#include "Type.hpp"

namespace rfl::parsing::schema {

template <class R, class W, class T, class ProcessorsType>
Definition make() {
  std::map<std::string, Type> definitions;
  auto root = Parser<R, W, T, ProcessorsType>::to_schema(&definitions);
  return Definition{.root_ = root, .definitions_ = definitions};
}

}  // namespace rfl::parsing::schema

#endif
