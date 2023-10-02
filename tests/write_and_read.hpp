#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <iostream>
#include <rfl/json.hpp>
#include <string>

template <class T>
void write_and_read(const T& _struct, const std::string& _expected) {
    const auto json_string1 = rfl::json::write(_struct);
    if (json_string1 != _expected) {
        std::cout << "Test failed on write. Expected:" << std::endl
                  << _expected << std::endl
                  << "Got: " << std::endl
                  << json_string1 << std::endl;
        return;
    }
    const auto struct2 = rfl::json::read<T>(json_string1).value();
    const auto json_string2 = rfl::json::write(struct2);
    if (json_string2 != _expected) {
        std::cout << "Test failed on read. Expected:" << std::endl
                  << _expected << std::endl
                  << "Got: " << std::endl
                  << json_string2 << std::endl;
        return;
    }
    std::cout << "OK" << std::endl << std::endl;
}

#endif
