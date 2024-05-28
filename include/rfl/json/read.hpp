#ifndef RFL_JSON_READ_HPP_
#define RFL_JSON_READ_HPP_

#include <yyjson.h>

#include <istream>
#include <string>

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
Result<internal::wrap_in_rfl_array_t<T>> read(const std::string& _json_str) {
  yyjson_doc* doc = yyjson_read(_json_str.c_str(), _json_str.size(), 0);
  if (!doc) {
    return Error("Could not parse document");
  }
  yyjson_val* root = yyjson_doc_get_root(doc);
  const auto r = Reader();
  if constexpr (std::is_class_v<T> && std::is_aggregate_v<T>) {
    using ProcessorsType = Processors<Ps...>;
    alignas(T) unsigned char buf[sizeof(T)];
    auto ptr = reinterpret_cast<T*>(buf);
    auto view = ProcessorsType::template process<T>(to_view(*ptr));
    using ViewType = std::remove_cvref_t<decltype(view)>;
    const auto err = Parser<ViewType, ProcessorsType>::read_view(
        r, InputVarType(root), &view);
    yyjson_doc_free(doc);
    if (err) [[unlikely]] {
      return *err;
    }
    return std::move(*ptr);
  } else {
    auto res = Parser<T, Processors<Ps...>>::read(r, InputVarType(root));
    yyjson_doc_free(doc);
    return res;
  }
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
