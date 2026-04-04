#ifndef RFL_BOOST_SERIALIZATION_WRITER_HPP_
#define RFL_BOOST_SERIALIZATION_WRITER_HPP_

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

#include "../always_false.hpp"
#include "../internal/is_literal.hpp"

namespace rfl::boost_serialization {

template <class OArchive>
class Writer {
 public:
  struct BoostOutputArray {};

  struct BoostOutputMap {};

  struct BoostOutputObject {};

  struct BoostOutputUnion {};

  struct BoostOutputVar {};

  using OutputArrayType = BoostOutputArray;
  using OutputMapType = BoostOutputMap;
  using OutputObjectType = BoostOutputObject;
  using OutputUnionType = BoostOutputUnion;
  using OutputVarType = BoostOutputVar;

  Writer(OArchive* _ar);

  ~Writer() = default;

  OutputArrayType array_as_root(const size_t _size) const;

  OutputMapType map_as_root(const size_t _size) const;

  OutputObjectType object_as_root(const size_t _size) const;

  OutputVarType null_as_root() const;

  OutputUnionType union_as_root() const;

  template <class T>
  OutputVarType value_as_root(const T& _var) const {
    new_value(_var);
    return OutputVarType{};
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const;

  OutputArrayType add_array_to_map(const std::string_view& _name,
                                   const size_t _size,
                                   OutputMapType* _parent) const;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const;

  OutputArrayType add_array_to_union(const size_t _index, const size_t _size,
                                     OutputUnionType* _parent) const;

  OutputMapType add_map_to_array(const size_t _size,
                                 OutputArrayType* _parent) const;

  OutputMapType add_map_to_map(const std::string_view& _name,
                               const size_t _size,
                               OutputMapType* _parent) const;

  OutputMapType add_map_to_object(const std::string_view& _name,
                                  const size_t _size,
                                  OutputObjectType* _parent) const;

  OutputMapType add_map_to_union(const size_t _index, const size_t _size,
                                 OutputUnionType* _parent) const;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const;

  OutputObjectType add_object_to_map(const std::string_view& _name,
                                     const size_t _size,
                                     OutputMapType* _parent) const;

  OutputObjectType add_object_to_object(const std::string_view& _name,
                                        const size_t _size,
                                        OutputObjectType* _parent) const;

  OutputObjectType add_object_to_union(const size_t _index, const size_t _size,
                                       OutputUnionType* _parent) const;

  OutputUnionType add_union_to_array(OutputArrayType* _parent) const;

  OutputUnionType add_union_to_map(const std::string_view& _name,
                                   OutputMapType* _parent) const;

  OutputUnionType add_union_to_object(const std::string_view& _name,
                                      OutputObjectType* _parent) const;

  OutputUnionType add_union_to_union(const size_t _index,
                                     OutputUnionType* _parent) const;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* /*_parent*/) const {
    new_value(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_map(const std::string_view& _name, const T& _var,
                                 OutputMapType* /*_parent*/) const {
    write_key(_name);
    new_value(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& /*_name*/,
                                    const T& _var,
                                    OutputObjectType* /*_parent*/) const {
    new_value(_var);
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_union(const size_t _index, const T& _var,
                                   OutputUnionType* /*_parent*/) const {
    write_index(_index);
    new_value(_var);
    return OutputVarType{};
  }

  OutputVarType add_null_to_array(OutputArrayType* _parent) const;

  OutputVarType add_null_to_map(const std::string_view& _name,
                                OutputMapType* _parent) const;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const;

  OutputVarType add_null_to_union(const size_t _index,
                                  OutputUnionType* _parent) const;

  void end_array(OutputArrayType* _arr) const;

  void end_map(OutputMapType* _obj) const;

  void end_object(OutputObjectType* _obj) const;

 private:
  void write_size(const size_t _size) const;

  void write_index(const size_t _index) const;

  void write_key(const std::string_view& _name) const;

  template <class T>
  void new_value(const T& _var) const {
    using Type = std::remove_cvref_t<T>;
    if constexpr (std::is_same<Type, std::string>()) {
      *ar_ << _var;
    } else if constexpr (std::is_same<Type, bool>()) {
      *ar_ << _var;
    } else if constexpr (std::is_floating_point<Type>()) {
      *ar_ << static_cast<double>(_var);
    } else if constexpr (std::is_unsigned<Type>()) {
      *ar_ << static_cast<std::uint64_t>(_var);
    } else if constexpr (std::is_integral<Type>()) {
      *ar_ << static_cast<std::int64_t>(_var);
    } else if constexpr (internal::is_literal_v<Type>) {
      *ar_ << static_cast<std::int64_t>(_var.value());
    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

 private:
  OArchive* ar_;
};

}  // namespace rfl::boost_serialization

#endif
