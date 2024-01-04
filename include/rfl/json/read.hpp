#ifndef RFL_JSON_READ_HPP_
#define RFL_JSON_READ_HPP_

#include <yyjson.h>

#include <istream>
#include <string>

#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl {
namespace json {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from a JSON var.
template <class T>
auto read(const InputVarType& _obj) {
  const auto r = Reader();
  return Parser<T>::read(r, _obj);
}

/// Parses an object from JSON using reflection.
template <class T>
Result<internal::wrap_in_rfl_array_t<T>> read(const std::string& _json_str) {
  using PtrType = std::unique_ptr<yyjson_doc, void (*)(yyjson_doc*)>;
  yyjson_doc* doc = yyjson_read(_json_str.c_str(), _json_str.size(), 0);
  if (!doc) {
    return Error("Could not parse document!");
  }
  const auto ptr = PtrType(doc, yyjson_doc_free);
  return read<T>(InputVarType(yyjson_doc_get_root(doc)));
}

/// Parses an object from a stringstream.
template <class T>
auto read(std::istream& _stream) {
  const auto json_str = std::string(std::istreambuf_iterator<char>(_stream),
                                    std::istreambuf_iterator<char>());
  return read<T>(json_str);
}

}  // namespace json
}  // namespace rfl

#endif
