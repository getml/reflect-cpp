#ifndef FLEXBUF_OUTPUTVAR_HPP_
#define FLEXBUF_OUTPUTVAR_HPP_

#include <flatbuffers/flexbuffers.h>

#include <optional>
#include <string>

namespace rfl {
namespace flexbuf {

class OutputVar {
 public:
  virtual ~OutputVar() = default;

  /// Inserts the value into the buffer.
  virtual void insert(const std::optional<std::string>& _key,
                      flexbuffers::Builder* _fbb) = 0;

  /// Whether this is null.
  virtual bool is_null() const = 0;
};

}  // namespace flexbuf
}  // namespace rfl

#endif
