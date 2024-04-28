#ifndef RFL_TOML_WRITE_HPP_
#define RFL_TOML_WRITE_HPP_

#include <ostream>
#include <sstream>
#include <string>
#include <toml++/toml.hpp>
#include <type_traits>

#include "../parsing/Parent.hpp"
#include "Parser.hpp"

namespace rfl::toml {

/// Writes a TOML into an ostream.
template <class T>
std::ostream& write(const T& _obj, std::ostream& _stream) {
  using ParentType = parsing::Parent<Writer>;
  ::toml::table root;
  auto w = Writer(&root);
  Parser<T>::write(w, _obj, typename ParentType::Root{});
  _stream << root;
  return _stream;
}

/// Returns a TOML string.
template <class T>
std::string write(const T& _obj) {
  using ParentType = parsing::Parent<Writer>;
  std::stringstream sstream;
  ::toml::table root;
  auto w = Writer(&root);
  Parser<T>::write(w, _obj, typename ParentType::Root{});
  sstream << root;
  return sstream.str();
}

}  // namespace rfl::toml

#endif
