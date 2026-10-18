#include "rfl/capnproto/Writer.hpp"

#include <stdexcept>

#include "rfl/parsing/schemaful/IsSchemafulWriter.hpp"

namespace rfl::capnproto {
namespace {
kj::StringPtr to_kj_string_ptr(const std::string_view& _str) {
  return {_str.data(), _str.size()};
}
template <class Set>
void set_basic_value(const auto& _value, const Set& _set) {
  std::visit(
      [&](const auto& _v) {
        using T = std::remove_cvref_t<decltype(_v)>;
        if constexpr (std::is_same_v<T, std::string>)
          _set(capnp::DynamicValue::Reader(_v.c_str()));
        else if constexpr (std::is_same_v<T, std::vector<char>>) {
          const auto bytes = kj::ArrayPtr<const kj::byte>(
              internal::ptr_cast<const kj::byte*>(_v.data()), _v.size());
          _set(capnp::DynamicValue::Reader(capnp::Data::Reader(bytes)));
        } else
          _set(capnp::DynamicValue::Reader(_v));
      },
      _value);
}
}  // namespace

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>,
              "This must be a schemaful writer.");

inline unsigned int SafeSizeToUInt(size_t sizeValue) {
  if (sizeValue >
      static_cast<size_t>(std::numeric_limits<unsigned int>::max())) {
    throw std::overflow_error(
        "size_t value is too big to fit into an unsigned int.");
  }
  return static_cast<unsigned int>(sizeValue);
}

Writer::~Writer() = default;

Writer::OutputObjectType Writer::object_as_root(const size_t /*_size*/) const {
  return OutputObjectType{static_cast<capnp::DynamicStruct::Builder*>(root_)
                              ->as<capnp::DynamicStruct>()};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const {
  return OutputArrayType{
      _parent->val_.native()
          .init(SafeSizeToUInt(_parent->ix_++), SafeSizeToUInt(_size))
          .as<capnp::DynamicList>()};
}

Writer::OutputArrayType Writer::add_array_to_map(const std::string_view& _name,
                                                 const size_t _size,
                                                 OutputMapType* _parent) const {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.native().get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto new_entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &new_entry);
  return add_array_to_object("value", _size, &new_entry);
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const {
  return OutputArrayType{
      _parent->val_.native()
          .init(to_kj_string_ptr(_name), SafeSizeToUInt(_size))
          .as<capnp::DynamicList>()};
}

Writer::OutputArrayType Writer::add_array_to_union(
    const size_t _index, const size_t _size, OutputUnionType* _parent) const {
  const auto field =
      _parent->val_.native().getSchema().getFields()[SafeSizeToUInt(_index)];
  return OutputArrayType{_parent->val_.native()
                             .init(field, SafeSizeToUInt(_size))
                             .template as<capnp::DynamicList>()};
}

Writer::OutputMapType Writer::add_map_to_array(const size_t _size,
                                               OutputArrayType* _parent) const {
  auto new_map = add_object_to_array(1, _parent);
  add_array_to_object("entries", _size, &new_map);
  return OutputMapType{new_map.val_};
}

Writer::OutputMapType Writer::add_map_to_map(const std::string_view& _name,
                                             const size_t _size,
                                             OutputMapType* _parent) const {
  auto new_map = add_object_to_map(_name, 1, _parent);
  add_array_to_object("entries", _size, &new_map);
  return OutputMapType{new_map.val_};
}

Writer::OutputMapType Writer::add_map_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const {
  auto new_map = add_object_to_object(_name, 1, _parent);
  add_array_to_object("entries", _size, &new_map);
  return OutputMapType{new_map.val_};
}

Writer::OutputMapType Writer::add_map_to_union(const size_t _index,
                                               const size_t _size,
                                               OutputUnionType* _parent) const {
  auto new_map = add_object_to_union(_index, 1, _parent);
  add_array_to_object("entries", _size, &new_map);
  return OutputMapType{new_map.val_};
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t /*_size*/, OutputArrayType* _parent) const {
  return OutputObjectType{_parent->val_.native()[SafeSizeToUInt(_parent->ix_++)]
                              .as<capnp::DynamicStruct>()};
}

Writer::OutputObjectType Writer::add_object_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.native().get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto new_entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &new_entry);
  return add_object_to_object("value", _size, &new_entry);
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t /*_size*/,
    OutputObjectType* _parent) const {
  return OutputObjectType{_parent->val_.native()
                              .get(to_kj_string_ptr(_name))
                              .as<capnp::DynamicStruct>()};
}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t /*_size*/,
    OutputUnionType* _parent) const {
  const auto field =
      _parent->val_.native().getSchema().getFields()[SafeSizeToUInt(_index)];
  return OutputObjectType{
      _parent->val_.native().init(field).template as<capnp::DynamicStruct>()};
}

Writer::OutputUnionType Writer::add_union_to_array(
    OutputArrayType* _parent) const {
  return OutputUnionType{_parent->val_.native()[SafeSizeToUInt(_parent->ix_++)]
                             .as<capnp::DynamicStruct>()};
}

Writer::OutputUnionType Writer::add_union_to_map(const std::string_view& _name,
                                                 OutputMapType* _parent) const {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.native().get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto new_entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &new_entry);
  return add_union_to_object("value", &new_entry);
}

Writer::OutputUnionType Writer::add_union_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const {
  return OutputUnionType{_parent->val_.native()
                             .get(to_kj_string_ptr(_name))
                             .as<capnp::DynamicStruct>()};
}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const {
  const auto field =
      _parent->val_.native().getSchema().getFields()[SafeSizeToUInt(_index)];
  return OutputUnionType{
      _parent->val_.native().init(field).template as<capnp::DynamicStruct>()};
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const {
  _parent->val_.native().set(SafeSizeToUInt(_parent->ix_++), capnp::VOID);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_map(const std::string_view& _name,
                                              OutputMapType* _parent) const {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.native().get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto new_entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &new_entry);
  return add_null_to_object("value", &new_entry);
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const {
  _parent->val_.native().set(to_kj_string_ptr(_name), capnp::VOID);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_union(
    const size_t _index, OutputUnionType* _parent) const {
  const auto field =
      _parent->val_.native().getSchema().getFields()[SafeSizeToUInt(_index)];
  _parent->val_.native().set(field, capnp::VOID);
  return OutputVarType{};
}

Writer::OutputObjectType Writer::map_entry(const std::string_view& _name,
                                           OutputMapType* _parent) const {
  auto entries = OutputArrayType{
      .val_ = _parent->val_.native().get("entries").as<capnp::DynamicList>(),
      .ix_ = _parent->ix_++};
  auto entry = add_object_to_array(2, &entries);
  add_value_to_object("key", std::string(_name), &entry);
  return entry;
}
Writer::OutputVarType Writer::set_array(const BasicValue& _value,
                                        OutputArrayType* _parent) const {
  const auto ix = SafeSizeToUInt(_parent->ix_++);
  set_basic_value(_value,
                  [&](const auto& _v) { _parent->val_.native().set(ix, _v); });
  return {};
}
Writer::OutputVarType Writer::set_object(const std::string_view& _name,
                                         const BasicValue& _value,
                                         OutputObjectType* _parent) const {
  set_basic_value(_value, [&](const auto& _v) {
    _parent->val_.native().set(to_kj_string_ptr(_name), _v);
  });
  return {};
}
Writer::OutputVarType Writer::set_union(size_t _index, const BasicValue& _value,
                                        OutputUnionType* _parent) const {
  const auto field =
      _parent->val_.native().getSchema().getFields()[SafeSizeToUInt(_index)];
  set_basic_value(
      _value, [&](const auto& _v) { _parent->val_.native().set(field, _v); });
  return {};
}
}  // namespace rfl::capnproto
