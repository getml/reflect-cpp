#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_custom_class2() {
    std::cout << "test_custom_class2" << std::endl;

    struct FiveDigitCode {
        using ReflectionType = int;

        FiveDigitCode(const int _code) : code(_code) {
            if (_code < 10000) {
                throw std::runtime_error("Less than five digits.");
            }
            if (_code > 99999) {
                throw std::runtime_error("More than five digits.");
            }
        }

        ReflectionType reflection() const { return code; };

       private:
        int code;
    };

    const auto result = rfl::json::read<FiveDigitCode>("123");

    if (result) {
        std::cout << "Failed: Expected an error, but got none." << std::endl;
        return;
    }

    const auto c = FiveDigitCode(12345);

    write_and_read(c, "12345");
}
