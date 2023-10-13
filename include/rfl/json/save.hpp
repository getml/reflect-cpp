#ifndef RFL_JSON_SAVE_HPP_
#define RFL_JSON_SAVE_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "rfl/Result.hpp"
#include "rfl/json/write.hpp"

namespace rfl {
namespace json {

template <class T>
Result<Nothing> save(const std::string& _fname, const T& _obj) {
    try {
        std::ofstream outfile;
        outfile.open(_fname);
        outfile << write(_obj);
        outfile.close();
    } catch (std::exception& e) {
        return Error(e.what());
    }
    return Nothing{};
}

}  // namespace json
}  // namespace rfl

#endif
