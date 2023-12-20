#ifndef RFL_XML_OUTPUTARRAY_HPP_
#define RFL_XML_OUTPUTARRAY_HPP_

#include <pugixml.hpp>
#include <string>
#include <vector>

#include "rfl/Ref.hpp"
#include "rfl/xml/OutputVar.hpp"

namespace rfl {
namespace xml {

class OutputArray : public OutputVar {
 public:
  OutputArray() {}

  ~OutputArray() = default;

  /// Inserts all elements into the builder.
  void insert(const std::string& _key, pugi::xml_node* _parent) final {
    for (auto& v : vars_) {
      v->insert(_key, _parent);
    }
  };

  /// Whether this is null.
  bool is_null() const final { return false; }

  /// Adds a new element to the vector.
  void push_back(const Ref<OutputVar>& _var) { vars_.push_back(_var); }

 private:
  /// The underlying variables.
  std::vector<Ref<OutputVar>> vars_;
};

}  // namespace xml
}  // namespace rfl

#endif
