#ifndef RFL_XML_OUTPUTVALUE_HPP_
#define RFL_XML_OUTPUTVALUE_HPP_

#include <pugixml.hpp>
#include <string>
#include <utility>

#include "../always_false.hpp"
#include "OutputVar.hpp"

namespace rfl {
namespace xml {

template <class T>
class OutputValue : public OutputVar {
 public:
  static constexpr const char* XML_CONTENT = "xml_content";

 public:
  OutputValue(T _val, bool _is_attribute)
      : is_attribute_(_is_attribute), val_(_val) {}

  ~OutputValue() = default;

  /// Inserts all elements into the builder.
  void insert(const std::string& _key, pugi::xml_node* _parent) final {
    const auto str = to_string();
    if (is_attribute_) {
      _parent->append_attribute(_key.c_str()) = str.c_str();
    } else if (_key == XML_CONTENT) {
      _parent->append_child(pugi::node_pcdata).set_value(str.c_str());
    } else {
      auto node_child = _parent->append_child(_key.c_str());
      node_child.append_child(pugi::node_pcdata).set_value(str.c_str());
    }
  };

  /// Whether this is null.
  bool is_null() const final { return false; }

 private:
  /// Transforms the underlying value into a string.
  std::string to_string() {
    if constexpr (std::is_same<std::decay_t<T>, std::string>()) {
      return val_;
    } else if constexpr (std::is_same<std::decay_t<T>, bool>()) {
      return val_ ? "true" : "false";
    } else if constexpr (std::is_floating_point<std::decay_t<T>>() ||
                         std::is_integral<std::decay_t<T>>()) {
      return std::to_string(val_);
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
  }

 private:
  /// Whether you want this encoded as an attribute.
  bool is_attribute_;

  /// The underlying value.
  T val_;
};

}  // namespace xml
}  // namespace rfl

#endif
