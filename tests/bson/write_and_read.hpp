#ifndef WRITE_AND_READ_
#define WRITE_AND_READ_

#include <iostream>
#include <rfl/bson.hpp>
#include <string>

template <class T>
void write_and_read(const T& _struct) {
  const auto bytes1 = rfl::bson::write(_struct);

  const auto res = rfl::bson::read<T>(bytes1);

  if (!res) {
    std::cout << "Test failed on read. Error: " << res.error().value().what()
              << std::endl
              << std::endl;
    return;
  }

  const auto bytes2 = rfl::bson::write(res.value());

  if (bytes1.size() != bytes2.size()) {
    std::cout << "Test failed on write. Number of bytes was different."
              << std::endl
              << std::endl;
    return;
  }

  if (bytes1 != bytes2) {
    std::cout << "Test failed on write. Content was not identical." << std::endl
              << std::endl;
    return;
  }

  std::cout << "OK" << std::endl << std::endl;
}

#endif
