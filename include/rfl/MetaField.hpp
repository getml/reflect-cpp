#ifndef RFL_METAFIELD_HPP_
#define RFL_METAFIELD_HPP_

#include <string>

namespace rfl {

/// Contains meta-information about a field in a struct.
class MetaField {
 public:
  MetaField(const std::string& _name, const std::string& _type)
      : name_(_name), type_(_type) {}

  ~MetaField() = default;

  /// The name of the field we describe.
  const std::string& name() const { return name_; };

  /// The type of the field we describe.
  const std::string& type() const { return type_; };

 private:
  /// The name of the field we describe.
  std::string name_;

  /// The type of the field we describe.
  std::string type_;
};

}  // namespace rfl

#endif  // RFL_TAGGEDUNION_HPP_
