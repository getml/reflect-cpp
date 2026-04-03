#ifndef RFL_YAS_WRITE_HPP_
#define RFL_YAS_WRITE_HPP_

#include <string>
#include <vector>
#include <yas/binary_oarchive.hpp>
#include <yas/binary_iarchive.hpp>
#include <yas/mem_streams.hpp>
#include <yas/std_types.hpp>

#include "../Processors.hpp"
#include "../Result.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Writer.hpp"

namespace rfl::yas {

/// Writes an object to a yas OutputArchive.
template <class IArchive = yas::binary_iarchive<yas::mem_istream>, class OArchive,
          class... Ps>
void write_to_archive(OArchive& _archive, const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using ParentType = parsing::Parent<Writer<OArchive>>;
  auto w = Writer<OArchive>(&_archive);
  Parser<yas::Reader<IArchive>, yas::Writer<OArchive>, T,
         Processors<Ps...>>::write(w, _obj, typename ParentType::Root{});
}

/// Returns yas binary bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  yas::mem_ostream os;
  yas::binary_oarchive<yas::mem_ostream> ar(os);
  write_to_archive<yas::binary_iarchive<yas::mem_istream>,
                   yas::binary_oarchive<yas::mem_ostream>, Ps...>(ar, _obj);
  auto buf = os.get_intrusive_buffer();
  return std::vector<char>(buf.data, buf.data + buf.size);
}

}  // namespace rfl::yas

#endif
