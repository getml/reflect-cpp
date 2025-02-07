#ifndef GENERIC_WRITER_HPP_
#define GENERIC_WRITER_HPP_

#include <cstddef>
#include <exception>
#include <functional>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Generic.hpp"
#include "../Result.hpp"
#include "../Variant.hpp"
#include "../always_false.hpp"

namespace rfl::generic {

struct Writer {
  struct OutputArray {
    Generic::Array* val_;
  };

  struct OutputObject {
    Generic::Object* val_;
  };

  using OutputArrayType = OutputArray;
  using OutputObjectType = OutputObject;
  using OutputVarType = Generic;

  Writer() {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const noexcept;

  OutputObjectType object_as_root(const size_t _size) const noexcept;

  OutputVarType null_as_root() const noexcept;

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    root_ = to_generic(_var);
    return root_;
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const noexcept;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const noexcept;

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    const auto g = to_generic(_var);
    _parent->val_->push_back(g);
    return g;
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    const auto g = to_generic(_var);
    _parent->val_->insert(_name, g);
    return g;
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept;

  void end_array(OutputArrayType*) const noexcept {}

  void end_object(OutputObjectType*) const noexcept {}

  OutputVarType& root() { return root_; }

 private:
  template <class T>
  OutputVarType to_generic(const T& _var) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return OutputVarType(_var);
    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>()) {
      return OutputVarType(_var);
    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>()) {
      return OutputVarType(static_cast<double>(_var));
    } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
      return OutputVarType(static_cast<int64_t>(_var));
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
    return OutputVarType{};
  }

 private:
  mutable OutputVarType root_;
};

}  // namespace rfl::generic

#endif
