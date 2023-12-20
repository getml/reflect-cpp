#ifndef RFL_XML_OUTPUTNULL_HPP_
#define RFL_XML_OUTPUTNULL_HPP_

#include <optional>
#include <pugixml.hpp>
#include <string>

#include "rfl/Ref.hpp"
#include "rfl/xml/OutputVar.hpp"

namespace rfl {
namespace xml {

class OutputNull : public OutputVar {
 public:
  OutputNull() {}

  ~OutputNull() = default;

  /// Inserts all elements into the builder.
  void insert(const std::string& _key, pugi::xml_node* _parent) final {
    _parent->append_child(_key.c_str());
  }

  /// Whether this is null.
  bool is_null() const final { return true; }
};

}  // namespace xml
}  // namespace rfl

#endif
