#ifndef RFL_IO_SAVE_STRING_HPP_
#define RFL_IO_SAVE_STRING_HPP_

#include <fstream>
#include <iostream>
#include <string>

#include "rfl/Result.hpp"

namespace rfl {
namespace io {

Result<Nothing> save_string(const std::string& _fname,
                            const std::string& _str) {
    try {
        std::ofstream outfile;
        outfile.open(_fname);
        outfile << _str;
        outfile.close();
    } catch (std::exception& e) {
        return Error(e.what());
    }
    return Nothing{};
}

}  // namespace io
}  // namespace rfl

#endif
