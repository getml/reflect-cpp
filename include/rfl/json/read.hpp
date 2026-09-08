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
/// A JSON var is the internal representation used by the yyjson library.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _obj The JSON variant object to parse from
/// @return Result containing either the parsed object of type T or an error
template <class T, class... Ps>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T, Processors<Ps...>>::read(r, _obj);
}

/// Parses an object from JSON using reflection.
/// This function reads a JSON string and constructs a C++ object using compile-time reflection.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _json_str The JSON string to parse
/// @param _flag Optional yyjson flags for parsing (default: 0). Note: YYJSON_READ_INSITU is not supported.
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const std::string_view _json_str, const yyjson_read_flag _flag = 0) {
  if (_flag & YYJSON_READ_INSITU) {
    return error("YYJSON_READ_INSITU is not supported");
  }
  yyjson_read_err err;
  // According to yyjson's doc, it's safe castaway constness as long as
  // YYJSON_READ_INSITU is not set
  yyjson_doc* doc = yyjson_read_opts(const_cast<char*>(_json_str.data()),
                                     _json_str.size(), _flag, NULL, &err);
  if (!doc) {
    return error("Could not parse document: " + std::string(err.msg));
  }
  yyjson_val* root = yyjson_doc_get_root(doc);
  const auto r = Reader();
  auto res = Parser<T, Processors<Ps...>>::read(r, InputVarType(root));
  yyjson_doc_free(doc);
  return res;
}

/// Parses an object from a stringstream.
/// Reads the entire stream content and parses it as JSON.
/// @tparam T The type to parse into
/// @tparam Ps Processors to apply during parsing (transforms the data)
/// @param _stream The input stream containing JSON data
/// @param _flag Optional yyjson flags for parsing (default: 0)
/// @return Result containing either the parsed object (or array of objects) or an error message
template <class T, class... Ps>
auto read(std::istream& _stream, const yyjson_read_flag _flag = 0) {
  const auto json_str = std::string(std::istreambuf_iterator<char>(_stream),
                                    std::istreambuf_iterator<char>());
  return read<T, Ps...>(json_str, _flag);
}

}  // namespace json
}  // namespace rfl

#endif
