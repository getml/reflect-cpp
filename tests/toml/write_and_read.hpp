#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <iostream>
#include <rfl/yaml.hpp>
#include <string>

template <class T>
void write_and_read(const T& _struct) {
  const auto yaml_string1 = rfl::yaml::write(_struct);
  const auto res = rfl::yaml::read<T>(yaml_string1);
  if (!res) {
    std::cout << "Test failed on read. Error: " << res.error().value().what()
              << std::endl
              << "Original string: " << yaml_string1 << std::endl
              << std::endl;
    return;
  }
  const auto yaml_string2 = rfl::yaml::write(res.value());
  if (yaml_string2 != yaml_string1) {
    std::cout << "Test failed on read. Expected:" << std::endl
              << yaml_string1 << std::endl
              << "Got: " << std::endl
              << yaml_string2 << std::endl
              << std::endl;
    return;
  }
  // std::cout << yaml_string1 << std::endl;
  std::cout << "OK" << std::endl << std::endl;
}

#endif
