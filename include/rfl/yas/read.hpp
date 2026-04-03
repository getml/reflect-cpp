#ifndef RFL_YAS_READ_HPP_
#define RFL_YAS_READ_HPP_

#include <istream>
#include <string>
#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/mem_streams.hpp>
#include <yas/std_types.hpp>

#include "../Processors.hpp"
#include "../concepts.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::yas {

namespace detail {

template <class T, class IArchive, class OArchive, class... Ps>
auto read_from_archive(IArchive& _ar) {
  using R = Reader<IArchive>;
  auto r = R();
  auto var = typename R::InputVarType{&_ar};
  return Parser<yas::Reader<IArchive>, yas::Writer<OArchive>, T,
                Processors<Ps...>>::read(r, var);
}

}  // namespace detail

/// Reads from an existing yas input archive.
template <class T, class IArchive, class OArchive = yas::binary_oarchive<yas::mem_ostream>, class... Ps>
auto read_from_archive(IArchive& _ar) {
  return detail::read_from_archive<T, IArchive, OArchive, Ps...>(_ar);
}

/// Parses an object from bytes using a binary archive.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(
    const concepts::ByteLike auto* _bytes, const size_t _size) {
  try {
    yas::mem_istream is(_bytes, _size);
    yas::binary_iarchive<yas::mem_istream> ar(is);
    return detail::read_from_archive<T, yas::binary_iarchive<yas::mem_istream>,
                                     yas::binary_oarchive<yas::mem_ostream>,
                                     Ps...>(ar);
  } catch (std::exception& e) {
    return error(std::string("yas read error: ") + e.what());
  }
}

/// Parses an object from a byte container using a binary archive.
template <class T, class... Ps>
auto read(const concepts::ContiguousByteContainer auto& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

}  // namespace rfl::yas

#endif
