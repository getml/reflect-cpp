#ifndef RFL_JSON_LOAD_HPP_
#define RFL_JSON_LOAD_HPP_

#include "rfl/Result.hpp"
#include "rfl/io/load_string.hpp"
#include "rfl/json/read.hpp"

namespace rfl {
namespace json {

template <class T>
Result<T> load(const std::string& _fname) {
    return rfl::io::load_string(_fname).and_then(read<T>);
}

}  // namespace json
}  // namespace rfl

#endif
