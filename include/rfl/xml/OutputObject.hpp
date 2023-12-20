#ifndef RFL_XML_OUTPUTOBJECT_HPP_
#define RFL_XML_OUTPUTOBJECT_HPP_

#include <pugixml.hpp>
#include <string>
#include <utility>

#include "rfl/Ref.hpp"
#include "rfl/xml/OutputVar.hpp"

namespace rfl {
namespace xml {

class OutputObject : public OutputVar {
 public:
  OutputObject() {}

  ~OutputObject() = default;

  /// Inserts all elements into the builder.
  void insert(const std::string& _key, pugi::xml_node* _parent) final {
    auto node_child = _parent->append_child(_key.c_str());
    for (auto& v : vars_) {
      v.second->insert(v.first, &node_child);
    }
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

}  // namespace xml
}  // namespace rfl

#endif
