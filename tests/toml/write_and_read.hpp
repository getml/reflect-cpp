#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <iostream>
#include <rfl/toml.hpp>
#include <string>

template <class T>
void write_and_read(const T& _struct) {
  const auto toml_string1 = rfl::toml::write(_struct);
  const auto res = rfl::toml::read<T>(toml_string1);
  if (!res) {
    std::cout << "Test failed on read. Error: " << res.error().value().what()
              << std::endl
              << "Original string: " << toml_string1 << std::endl
              << std::endl;
    return;
  }
  const auto toml_string2 = rfl::toml::write(res.value());
  if (toml_string2 != toml_string1) {
    std::cout << "Test failed on read. Expected:" << std::endl
              << toml_string1 << std::endl
              << "Got: " << std::endl
              << toml_string2 << std::endl
              << std::endl;
    return;
  }
  // std::cout << toml_string1 << std::endl;
  std::cout << "OK" << std::endl << std::endl;
}

#endif
