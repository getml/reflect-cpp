#ifndef FLEXBUF_OUTPUTNULL_HPP_
#define FLEXBUF_OUTPUTNULL_HPP_

#include <flatbuffers/flexbuffers.h>

#include <optional>
#include <string>

#include "rfl/Ref.hpp"
#include "rfl/flexbuf/OutputVar.hpp"

namespace rfl {
namespace flexbuf {

class OutputNull : public OutputVar {
 public:
  OutputNull() {}

  ~OutputNull() = default;

  /// Inserts all elements into the builder.
  void insert(const std::optional<std::string>& _key,
              flexbuffers::Builder* _fbb) final {
    if (_key) {
      _fbb->Null(_key->c_str());
    } else {
      _fbb->Null();
    }
  }

  /// Whether this is null.
  bool is_null() const final { return true; }
};

}  // namespace flexbuf
}  // namespace rfl

#endif
