#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <iostream>
#include <rfl/xml.hpp>
#include <string>

#include "rfl/internal/StringLiteral.hpp"

template <
    rfl::internal::StringLiteral _root_name = rfl::internal::StringLiteral(""),
    class T>
void write_and_read(const T& _struct) {
  const auto xml_string1 = rfl::xml::write<_root_name>(_struct);
  const auto res = rfl::xml::read<T>(xml_string1);
  if (!res) {
    std::cout << "Test failed on read. Error: " << res.error().value().what()
              << std::endl
              << "Original string: " << xml_string1 << std::endl
              << std::endl;
    return;
  }
  const auto xml_string2 = rfl::xml::write<_root_name>(res.value());
  if (xml_string2 != xml_string1) {
    std::cout << "Test failed on read. Expected:" << std::endl
              << xml_string1 << std::endl
              << "Got: " << std::endl
              << xml_string2 << std::endl
              << std::endl;
    return;
  }
  // std::cout << xml_string1 << std::endl;
  std::cout << "OK" << std::endl << std::endl;
}

#endif
