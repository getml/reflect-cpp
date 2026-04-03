#ifndef RFL_BOOST_SERIALIZATION_READ_HPP_
#define RFL_BOOST_SERIALIZATION_READ_HPP_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <istream>
#include <streambuf>
#include <string>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::boost_serialization {

namespace detail {

template <class T, class IArchive, class OArchive, class... Ps>
auto read_from_archive(IArchive& _ar) {
  using R = Reader<IArchive>;
  auto r = R();
  auto var = typename R::InputVarType{&_ar};
  return Parser<IArchive, OArchive, T, Processors<Ps...>>::read(r, var);
}

/// A read-only streambuf that wraps an existing memory buffer without copying.
class MemBuf : public std::streambuf {
 public:
  MemBuf(const char* _data, size_t _size) {
    auto* p = const_cast<char*>(_data);
    setg(p, p, p + _size);
  }
};

}  // namespace detail

/// Reads from an existing Boost input archive.
template <class T, class IArchive, class OArchive, class... Ps>
auto read_from_archive(IArchive& _ar) {
  return detail::read_from_archive<T, IArchive, OArchive, Ps...>(_ar);
}

/// Parses an object from bytes using a binary archive.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ByteLike auto* _bytes, const size_t _size) {
  try {
    detail::MemBuf buf(reinterpret_cast<const char*>(_bytes), _size);
    std::istream stream(&buf);
    boost::archive::binary_iarchive ar(
        stream, boost::archive::no_header | boost::archive::no_tracking);
    return detail::read_from_archive<T, boost::archive::binary_iarchive,
                                     boost::archive::binary_oarchive, Ps...>(
        ar);
  } catch (std::exception& e) {
    return error(e.what());
  }
}

/// Parses an object from a byte container using a binary archive.
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream using a binary archive.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  try {
    boost::archive::binary_iarchive ar(
        _stream, boost::archive::no_header | boost::archive::no_tracking);
    return detail::read_from_archive<T, boost::archive::binary_iarchive,
                                     boost::archive::binary_oarchive, Ps...>(
        ar);
  } catch (std::exception& e) {
    return Result<internal::wrap_in_rfl_array_t<T>>(error(e.what()));
  }
}

}  // namespace rfl::boost_serialization

#endif
