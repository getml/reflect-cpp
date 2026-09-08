#ifndef RFL_PARSING_SCHEMA_MAKE_HPP_
#define RFL_PARSING_SCHEMA_MAKE_HPP_

#include <map>

#include "../Parser.hpp"
#include "Definition.hpp"
#include "Type.hpp"

namespace rfl::parsing::schema {

/**
 * @brief Generates the schema for the type T.
 *
 * @tparam R The reader type.
 * @tparam W The writer type.
 * @tparam T The type to generate the schema for.
 * @tparam ProcessorsType The processors to use.
 * @return The generated schema.
 */
template <class R, class W, class T, class ProcessorsType>
Definition make() {
  std::map<std::string, Type> definitions;
  auto root = Parser<R, W, T, ProcessorsType>::to_schema(&definitions);
  return Definition{.root_ = root, .definitions_ = definitions};
}

}  // namespace rfl::parsing::schema

#endif
