#ifndef RFL_JSON_WRITE_HPP_
#define RFL_JSON_WRITE_HPP_

#include <yyjson.h>

#include <ostream>
#include <sstream>
#include <string>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace json {

/// Convenient alias for the YYJSON pretty flag
inline constexpr yyjson_write_flag pretty = YYJSON_WRITE_PRETTY;

/// Returns a JSON string.
template <class T>
std::string write(const T& _obj, const yyjson_write_flag _flag = 0) {
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer(yyjson_mut_doc_new(NULL));
  Parser<T>::write(w, _obj, typename ParentType::Root{});
  const char* json_c_str = yyjson_mut_write(w.doc_, _flag, NULL);
  const auto json_str = std::string(json_c_str);
  free((void*)json_c_str);
  yyjson_mut_doc_free(w.doc_);
  return json_str;
}

/// Writes a JSON into an ostream.
template <class T>
std::ostream& write(const T& _obj, std::ostream& _stream,
                    const yyjson_write_flag _flag = 0) {
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer(yyjson_mut_doc_new(NULL));
  Parser<T>::write(w, _obj, typename ParentType::Root{});
  const char* json_c_str = yyjson_mut_write(w.doc_, _flag, NULL);
  _stream << json_c_str;
  free((void*)json_c_str);
  yyjson_mut_doc_free(w.doc_);
  return _stream;
}

}  // namespace json
}  // namespace rfl

#endif  // JSON_PARSER_HPP_
