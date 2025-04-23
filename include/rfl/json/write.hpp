#ifndef RFL_JSON_WRITE_HPP_
#define RFL_JSON_WRITE_HPP_

#include <stdexcept>
#if __has_include(<yyjson.h>)
#include <yyjson.h>
#else
#include "../thirdparty/yyjson.h"
#endif

#include <ostream>
#include <sstream>
#include <string>

#include "../Processors.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl {
namespace json {

/// Convenient alias for the YYJSON pretty flag
inline constexpr yyjson_write_flag pretty = YYJSON_WRITE_PRETTY;

/// Returns a JSON string.
template <class... Ps>
std::string write(const auto& _obj, const yyjson_write_flag _flag = 0) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer(yyjson_mut_doc_new(NULL));
  Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
  yyjson_write_err err;
  const char* json_c_str =
      yyjson_mut_write_opts(w.doc_, _flag, NULL, NULL, &err);
  if (!json_c_str) {
    throw std::runtime_error("An error occured while writing to JSON: " +
                             std::string(err.msg));
  }
  const auto json_str = std::string(json_c_str);
  free((void*)json_c_str);
  yyjson_mut_doc_free(w.doc_);
  return json_str;
}

/// Writes a JSON into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream,
                    const yyjson_write_flag _flag = 0) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer>;
  auto w = Writer(yyjson_mut_doc_new(NULL));
  Parser<T, Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
  yyjson_write_err err;
  const char* json_c_str =
      yyjson_mut_write_opts(w.doc_, _flag, NULL, NULL, &err);
  if (!json_c_str) {
    throw std::runtime_error("An error occured while writing to JSON: " +
                             std::string(err.msg));
  }
  _stream << json_c_str;
  free((void*)json_c_str);
  yyjson_mut_doc_free(w.doc_);
  return _stream;
}

}  // namespace json
}  // namespace rfl

#endif
