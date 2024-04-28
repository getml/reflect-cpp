#ifndef RFL_TOML_WRITER_HPP_
#define RFL_TOML_WRITER_HPP_

#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>
#include <type_traits>
#include <vector>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl::toml {

class Writer {
 public:
  struct TOMLArray {
    ::toml::array* val_;
  };

  struct TOMLObject {
    ::toml::table* val_;
  };

  struct TOMLVar {};

  using OutputArrayType = TOMLArray;
  using OutputObjectType = TOMLObject;
  using OutputVarType = TOMLVar;

  Writer(::toml::table* _root) : root_(_root) {}

  ~Writer() = default;

  template <class T>
  OutputArrayType array_as_root(const T _size) const noexcept {
    static_assert(rfl::always_false_v<T>,
                  "TOML only allows tables as the root element.");
    return OutputArrayType{nullptr};
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    return OutputObjectType{root_};
  }

  OutputVarType null_as_root() const noexcept { return OutputVarType{}; }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    static_assert(rfl::always_false_v<T>,
                  "TOML only allows tables as the root element.");
    return OutputVarType{};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    const auto i = _parent->val_->size();
    _parent->val_->push_back(::toml::array());
    return OutputArrayType{_parent->val_->at(i).as_array()};
  }

  OutputArrayType add_array_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    _parent->val_->emplace(_name, ::toml::array());
    return OutputArrayType{_parent->val_->at_path(_name).as_array()};
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    const auto i = _parent->val_->size();
    _parent->val_->push_back(::toml::table());
    return OutputObjectType{_parent->val_->at(i).as_table()};
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    _parent->val_->emplace(_name, ::toml::table());
    return OutputObjectType{_parent->val_->at_path(_name).as_table()};
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    _parent->val_->push_back(::toml::value(_var));
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    _parent->val_->emplace(_name, ::toml::value(_var));
    return OutputVarType{};
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept {
    _parent->val_->push_back(std::string(""));
    return OutputVarType{};
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept {
    _parent->val_->emplace(_name, ::toml::value(std::string("")));
    return OutputVarType{};
  }

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  ::toml::table* root_;
};

}  // namespace rfl::toml

#endif  // JSON_PARSER_HPP_
