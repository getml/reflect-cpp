#include "rfl/boost_serialization/Writer.hpp"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>

namespace rfl::boost_serialization {

template <class OArchive>
Writer<OArchive>::Writer(OArchive* _ar) : ar_(_ar) {}

template <class OArchive>
auto Writer<OArchive>::array_as_root(const size_t _size) const
    -> OutputArrayType {
  write_size(_size);
  return OutputArrayType{};
}

template <class OArchive>
auto Writer<OArchive>::map_as_root(const size_t _size) const -> OutputMapType {
  write_size(_size);
  return OutputMapType{};
}

template <class OArchive>
auto Writer<OArchive>::object_as_root(const size_t _size) const
    -> OutputObjectType {
  write_size(_size);
  return OutputObjectType{};
}

template <class OArchive>
auto Writer<OArchive>::null_as_root() const -> OutputVarType {
  return OutputVarType{};
}

template <class OArchive>
auto Writer<OArchive>::union_as_root() const -> OutputUnionType {
  return OutputUnionType{};
}

template <class OArchive>
auto Writer<OArchive>::add_array_to_array(const size_t _size,
                                          OutputArrayType* /*_parent*/) const
    -> OutputArrayType {
  write_size(_size);
  return OutputArrayType{};
}

template <class OArchive>
auto Writer<OArchive>::add_array_to_map(const std::string_view& _name,
                                        const size_t _size,
                                        OutputMapType* /*_parent*/) const
    -> OutputArrayType {
  write_key(_name);
  write_size(_size);
  return OutputArrayType{};
}

template <class OArchive>
auto Writer<OArchive>::add_array_to_object(const std::string_view& /*_name*/,
                                           const size_t _size,
                                           OutputObjectType* /*_parent*/) const
    -> OutputArrayType {
  write_size(_size);
  return OutputArrayType{};
}

template <class OArchive>
auto Writer<OArchive>::add_array_to_union(const size_t _index,
                                          const size_t _size,
                                          OutputUnionType* /*_parent*/) const
    -> OutputArrayType {
  write_index(_index);
  write_size(_size);
  return OutputArrayType{};
}

template <class OArchive>
auto Writer<OArchive>::add_map_to_array(const size_t _size,
                                        OutputArrayType* /*_parent*/) const
    -> OutputMapType {
  write_size(_size);
  return OutputMapType{};
}

template <class OArchive>
auto Writer<OArchive>::add_map_to_map(const std::string_view& _name,
                                      const size_t _size,
                                      OutputMapType* /*_parent*/) const
    -> OutputMapType {
  write_key(_name);
  write_size(_size);
  return OutputMapType{};
}

template <class OArchive>
auto Writer<OArchive>::add_map_to_object(const std::string_view& /*_name*/,
                                         const size_t _size,
                                         OutputObjectType* /*_parent*/) const
    -> OutputMapType {
  write_size(_size);
  return OutputMapType{};
}

template <class OArchive>
auto Writer<OArchive>::add_map_to_union(const size_t _index, const size_t _size,
                                        OutputUnionType* /*_parent*/) const
    -> OutputMapType {
  write_index(_index);
  write_size(_size);
  return OutputMapType{};
}

template <class OArchive>
auto Writer<OArchive>::add_object_to_array(const size_t _size,
                                           OutputArrayType* /*_parent*/) const
    -> OutputObjectType {
  write_size(_size);
  return OutputObjectType{};
}

template <class OArchive>
auto Writer<OArchive>::add_object_to_map(const std::string_view& _name,
                                         const size_t _size,
                                         OutputMapType* /*_parent*/) const
    -> OutputObjectType {
  write_key(_name);
  write_size(_size);
  return OutputObjectType{};
}

template <class OArchive>
auto Writer<OArchive>::add_object_to_object(const std::string_view& /*_name*/,
                                            const size_t _size,
                                            OutputObjectType* /*_parent*/) const
    -> OutputObjectType {
  write_size(_size);
  return OutputObjectType{};
}

template <class OArchive>
auto Writer<OArchive>::add_object_to_union(const size_t _index,
                                           const size_t _size,
                                           OutputUnionType* /*_parent*/) const
    -> OutputObjectType {
  write_index(_index);
  write_size(_size);
  return OutputObjectType{};
}

template <class OArchive>
auto Writer<OArchive>::add_union_to_array(OutputArrayType* /*_parent*/) const
    -> OutputUnionType {
  return OutputUnionType{};
}

template <class OArchive>
auto Writer<OArchive>::add_union_to_map(const std::string_view& _name,
                                        OutputMapType* /*_parent*/) const
    -> OutputUnionType {
  write_key(_name);
  return OutputUnionType{};
}

template <class OArchive>
auto Writer<OArchive>::add_union_to_object(const std::string_view& /*_name*/,
                                           OutputObjectType* /*_parent*/) const
    -> OutputUnionType {
  return OutputUnionType{};
}

template <class OArchive>
auto Writer<OArchive>::add_union_to_union(const size_t _index,
                                          OutputUnionType* /*_parent*/) const
    -> OutputUnionType {
  write_index(_index);
  return OutputUnionType{};
}

template <class OArchive>
auto Writer<OArchive>::add_null_to_array(OutputArrayType* /*_parent*/) const
    -> OutputVarType {
  return OutputVarType{};
}

template <class OArchive>
auto Writer<OArchive>::add_null_to_map(const std::string_view& _name,
                                       OutputMapType* /*_parent*/) const
    -> OutputVarType {
  write_key(_name);
  return OutputVarType{};
}

template <class OArchive>
auto Writer<OArchive>::add_null_to_object(const std::string_view& /*_name*/,
                                          OutputObjectType* /*_parent*/) const
    -> OutputVarType {
  return OutputVarType{};
}

template <class OArchive>
auto Writer<OArchive>::add_null_to_union(const size_t _index,
                                         OutputUnionType* /*_parent*/) const
    -> OutputVarType {
  write_index(_index);
  return OutputVarType{};
}

template <class OArchive>
void Writer<OArchive>::end_array(OutputArrayType* /*_arr*/) const {}

template <class OArchive>
void Writer<OArchive>::end_map(OutputMapType* /*_obj*/) const {}

template <class OArchive>
void Writer<OArchive>::end_object(OutputObjectType* /*_obj*/) const {}

template <class OArchive>
void Writer<OArchive>::write_size(const size_t _size) const {
  *ar_ << static_cast<std::uint64_t>(_size);
}

template <class OArchive>
void Writer<OArchive>::write_index(const size_t _index) const {
  *ar_ << static_cast<std::uint64_t>(_index);
}

template <class OArchive>
void Writer<OArchive>::write_key(const std::string_view& _name) const {
  *ar_ << std::string(_name);
}

// Explicit instantiations for common archive types.
template class Writer<boost::archive::binary_oarchive>;
template class Writer<boost::archive::text_oarchive>;

}  // namespace rfl::boost_serialization
