#ifndef RFL_JSON_LOAD_HPP_
#define RFL_JSON_LOAD_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "rfl/Result.hpp"
#include "rfl/json/read.hpp"

namespace rfl {
namespace json {

template <class T>
Result<T> load(const std::string& _fname) {
    std::ifstream infile(_fname);
    if (infile.is_open()) {
        auto r = read<T>(std::string(std::istreambuf_iterator<char>(infile),
                                     std::istreambuf_iterator<char>()));
        infile.close();
        return r;
    } else {
        return Error("Unable to open file '" + _fname +
                     "' or file could not be found.");
    }
}

}  // namespace json
}  // namespace rfl

#endif
