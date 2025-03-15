#ifndef RFL_JSON_READ_HPP_
#define RFL_JSON_READ_HPP_

#if __has_include(<yyjson.h>)
#include <yyjson.h>
#else
#include "../thirdparty/yyjson.h"
#endif

#include <istream>
#include <string_view>

#include "../Processors.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl {
namespace json {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from a JSON var.
template <class T, class... Ps>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _obj);
}

/// Parses an object from JSON using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const std::string_view _json_str, const yyjson_read_flag _flag = 0) {
  yyjson_doc* doc = yyjson_read(_json_str.data(), _json_str.size(), _flag);
  if (!doc) {
    return error("Could not parse document");
  }
  yyjson_val* root = yyjson_doc_get_root(doc);
  const auto r = Reader();
  auto res = Parser<T, Processors<Ps...>>::read(r, InputVarType(root));
  yyjson_doc_free(doc);
  return res;
}

/// Parses an object from a stringstream.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  const auto json_str = std::string(std::istreambuf_iterator<char>(_stream),
                                    std::istreambuf_iterator<char>());
  return read<T, Ps...>(json_str);
}

}  // namespace json
}  // namespace rfl

#endif
