#ifndef RFL_XML_OUTPUTVAR_HPP_
#define RFL_XML_OUTPUTVAR_HPP_

#include <pugixml.hpp>
#include <string>

namespace rfl {
namespace xml {

class OutputVar {
 public:
  virtual ~OutputVar() = default;

  /// Inserts the value into the _parent.
  virtual void insert(const std::string& _key, pugi::xml_node* _parent) = 0;

  /// Whether this is null.
  virtual bool is_null() const = 0;
};

}  // namespace xml
}  // namespace rfl

#endif
