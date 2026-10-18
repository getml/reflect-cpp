#include "rfl/cereal/Archive.hpp"

namespace rfl::cereal::detail {

InputArchive::InputArchive(std::istream& _stream)
    : archive_(new ::cereal::PortableBinaryInputArchive(_stream)) {}
InputArchive::~InputArchive() { delete archive_; }
::cereal::PortableBinaryInputArchive& InputArchive::get() const {
  return *archive_;
}

OutputArchive::OutputArchive(std::ostream& _stream)
    : archive_(new ::cereal::PortableBinaryOutputArchive(_stream)) {}
OutputArchive::~OutputArchive() { delete archive_; }
::cereal::PortableBinaryOutputArchive& OutputArchive::get() const {
  return *archive_;
}

void read_value(::cereal::PortableBinaryInputArchive* _archive, bool& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const bool& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive, char& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const char& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                signed char& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const signed char& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                unsigned char& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const unsigned char& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive, short& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const short& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                unsigned short& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const unsigned short& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive, int& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const int& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                unsigned int& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const unsigned int& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive, long& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const long& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                unsigned long& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const unsigned long& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                long long& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const long long& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                unsigned long long& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const unsigned long long& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive, float& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const float& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                double& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const double& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                long double& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const long double& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                wchar_t& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const wchar_t& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                char8_t& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const char8_t& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                char16_t& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const char16_t& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                char32_t& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const char32_t& _value) {
  (*_archive)(_value);
}
void read_value(::cereal::PortableBinaryInputArchive* _archive,
                std::string& _value) {
  (*_archive)(_value);
}
void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                 const std::string& _value) {
  (*_archive)(_value);
}

std::size_t read_size(::cereal::PortableBinaryInputArchive* _archive) {
  std::size_t size;
  (*_archive)(::cereal::make_size_tag(size));
  return size;
}

void write_size(::cereal::PortableBinaryOutputArchive* _archive,
                std::size_t _size) {
  (*_archive)(::cereal::make_size_tag(_size));
}

void read_bytes(::cereal::PortableBinaryInputArchive* _archive, void* _data,
                std::size_t _size) {
  (*_archive)(::cereal::binary_data(_data, _size));
}

void write_bytes(::cereal::PortableBinaryOutputArchive* _archive,
                 const void* _data, std::size_t _size) {
  (*_archive)(::cereal::binary_data(_data, _size));
}

}  // namespace rfl::cereal::detail
