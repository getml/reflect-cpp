#ifndef RFL_CEREAL_ARCHIVE_HPP_
#define RFL_CEREAL_ARCHIVE_HPP_

#ifndef RFL_CEREAL_MODULE_INTERFACE
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/string.hpp>
#endif

#include <cstddef>
#include <iosfwd>
#include <string>

#include "../common.hpp"

namespace cereal {
class PortableBinaryInputArchive;
class PortableBinaryOutputArchive;
}  // namespace cereal

namespace rfl::cereal::detail {

// Archive implementation stays in an ordinary translation unit. Imported
// templates only need the archive pointers and these non-template operations.
class RFL_API InputArchive {
 public:
  explicit InputArchive(std::istream& _stream);
  ~InputArchive();
  InputArchive(const InputArchive&) = delete;
  InputArchive& operator=(const InputArchive&) = delete;

  ::cereal::PortableBinaryInputArchive& get() const;

 private:
  ::cereal::PortableBinaryInputArchive* archive_;
};

class RFL_API OutputArchive {
 public:
  explicit OutputArchive(std::ostream& _stream);
  ~OutputArchive();
  OutputArchive(const OutputArchive&) = delete;
  OutputArchive& operator=(const OutputArchive&) = delete;

  ::cereal::PortableBinaryOutputArchive& get() const;

 private:
  ::cereal::PortableBinaryOutputArchive* archive_;
};

RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        bool& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const bool& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        char& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const char& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        signed char& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const signed char& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        unsigned char& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const unsigned char& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        short& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const short& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        unsigned short& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const unsigned short& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        int& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const int& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        unsigned int& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const unsigned int& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        long& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const long& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        unsigned long& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const unsigned long& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        long long& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const long long& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        unsigned long long& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const unsigned long long& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        float& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const float& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        double& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const double& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        long double& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const long double& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        wchar_t& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const wchar_t& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        char8_t& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const char8_t& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        char16_t& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const char16_t& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        char32_t& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const char32_t& _value);
RFL_API void read_value(::cereal::PortableBinaryInputArchive* _archive,
                        std::string& _value);
RFL_API void write_value(::cereal::PortableBinaryOutputArchive* _archive,
                         const std::string& _value);

RFL_API std::size_t read_size(::cereal::PortableBinaryInputArchive* _archive);
RFL_API void write_size(::cereal::PortableBinaryOutputArchive* _archive,
                        std::size_t _size);
RFL_API void read_bytes(::cereal::PortableBinaryInputArchive* _archive,
                        void* _data, std::size_t _size);
RFL_API void write_bytes(::cereal::PortableBinaryOutputArchive* _archive,
                         const void* _data, std::size_t _size);

}  // namespace rfl::cereal::detail

#endif
