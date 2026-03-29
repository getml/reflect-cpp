#ifndef RFL_CEREAL_WRITER_HPP_
#define RFL_CEREAL_WRITER_HPP_

#include <cereal/archives/portable_binary.hpp>
#include <cereal/cereal.hpp>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "../always_false.hpp"
#include "../common.hpp"

namespace rfl::cereal {

class Writer {
 public:
  using CerealArchive = ::cereal::PortableBinaryOutputArchive;

  struct CerealOutputArray {};

  struct CerealOutputObject {};

  struct CerealOutputVar {};

  using OutputArrayType = CerealOutputArray;
  using OutputObjectType = CerealOutputObject;
  using OutputVarType = CerealOutputVar;

  Writer(CerealArchive* _archive) : archive_(_archive) {}

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputArrayType{};
  }

  OutputObjectType object_as_root(const size_t _size) const {
    return OutputObjectType{};
  }

  OutputVarType null_as_root() const { return OutputVarType{}; }

  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    (*archive_)(_var);
    return OutputVarType{};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputArrayType{};
  }

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const {
    (*archive_)(::cereal::make_size_tag(_size));
    return OutputArrayType{};
  }

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const {
    return OutputObjectType{};
  }

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const {
    return OutputObjectType{};
  }

  template <class T>
  OutputVarType add_value_to_array(const T& _var, OutputArrayType*) const {
    (*archive_)(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var, OutputObjectType*) const {
    (*archive_)(::cereal::make_nvp(_name.data(), _var));
    return OutputVarType{};
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const {
    return OutputVarType{};
  }

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const {
    return OutputVarType{};
  }

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  CerealArchive* archive_;
};

}  // namespace rfl::cereal

#endif
