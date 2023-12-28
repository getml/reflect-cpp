#ifndef FLEXBUF_OUTPUTOBJECT_HPP_
#define FLEXBUF_OUTPUTOBJECT_HPP_

#include <flatbuffers/flexbuffers.h>

#include <optional>
#include <string>
#include <utility>

#include "../Ref.hpp"
#include "OutputVar.hpp"

namespace rfl {
namespace flexbuf {

class OutputObject : public OutputVar {
 public:
  OutputObject() {}

  ~OutputObject() = default;

  /// Inserts all elements into the builder.
  void insert(const std::optional<std::string>& _key,
              flexbuffers::Builder* _fbb) final {
    /// We have to catch the edge case of an empty map,
    /// because flexbuf ignores empty vectors/maps.
    if (vars_.size() == 0) {
      if (_key) {
        _fbb->Null(_key->c_str());
      } else {
        _fbb->Null();
      }
      return;
    }

    const auto start = _key ? _fbb->StartMap(_key->c_str()) : _fbb->StartMap();

    for (const auto& [name, elem] : vars_) {
      elem->insert(name, _fbb);
    }

    _fbb->EndMap(start);
  };

  /// Whether this is null.
  bool is_null() const final { return false; }

  /// Adds a new element to the vector.
  void push_back(const std::string& _name, const Ref<OutputVar>& _var) {
    vars_.push_back(std::make_pair(_name, _var));
  }

 private:
  /// The underlying variables.
  std::vector<std::pair<std::string, Ref<OutputVar>>> vars_;
};

}  // namespace flexbuf
}  // namespace rfl

#endif
