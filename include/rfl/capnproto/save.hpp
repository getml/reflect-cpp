#ifndef RFL_CAPNPROTO_SAVE_HPP_
#define RFL_CAPNPROTO_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "../Result.hpp"
#include "../io/save_bytes.hpp"
#include "write.hpp"

namespace rfl::capnproto {

template <class... Ps>
Result<Nothing> save(const std::string& _fname, const auto& _obj) {
  const auto write_func = [](const auto& _obj,
                             std::ostream& _stream) -> std::ostream& {
    return write<Ps...>(_obj, _stream);
  };
  return rfl::io::save_bytes(_fname, _obj, write_func);
}

}  // namespace rfl::capnproto

#endif
