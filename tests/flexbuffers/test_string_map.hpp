#include <iostream>
#include <map>
#include <memory>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>

#include "write_and_read.hpp"

void test_string_map() {
    std::cout << "test_string_map" << std::endl;

    std::map<std::string, std::unique_ptr<std::string>> homer;
    homer.insert(
        std::make_pair("firstName", std::make_unique<std::string>("Homer")));
    homer.insert(
        std::make_pair("lastName", std::make_unique<std::string>("Simpson")));

    write_and_read(homer);
}
