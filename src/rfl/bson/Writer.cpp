#include "rfl/bson/Writer.hpp"

namespace rfl::bson {

Writer::Writer(bson_t* _doc) : doc_(_doc) {}

Writer::~Writer() = default;

Writer::OutputArrayType Writer::array_as_root(
    const size_t _size) const noexcept {
  bson_array_builder_t* val = bson_array_builder_new();
  return OutputArrayType(val, IsRoot{});
}

Writer::OutputObjectType Writer::object_as_root(
    const size_t _size) const noexcept {
  return OutputObjectType(doc_, IsRoot{});
}

Writer::OutputVarType Writer::null_as_root() const noexcept {
  // Appears to be unsupported by the BSON C API.
  return OutputVarType{};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  bson_array_builder_t* val;
  bson_array_builder_append_array_builder_begin(_parent->val_, &val);
  return OutputArrayType(val, IsArray{_parent->val_});
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  bson_array_builder_t* val;
  bson_append_array_builder_begin(_parent->val_, _name.data(),
                                  static_cast<int>(_name.size()), &val);
  return OutputArrayType(val, IsObject{_parent->val_});
}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  subdocs_->emplace_back(rfl::Box<BSONType>());
  bson_array_builder_append_document_begin(_parent->val_,
                                           &(subdocs_->back()->val_));
  return OutputObjectType(&subdocs_->back()->val_, IsArray{_parent->val_});
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  subdocs_->emplace_back(rfl::Box<BSONType>());
  bson_append_document_begin(_parent->val_, _name.data(),
                             static_cast<int>(_name.size()),
                             &(subdocs_->back()->val_));
  return OutputObjectType(&subdocs_->back()->val_, IsObject{_parent->val_});
}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {
  bson_array_builder_append_null(_parent->val_);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  bson_append_null(_parent->val_, _name.data(), static_cast<int>(_name.size()));
  return OutputVarType{};
}

void Writer::end_array(OutputArrayType* _arr) const noexcept {
  const auto handle = [&](const auto _parent) {
    using Type = std::remove_cvref_t<decltype(_parent)>;
    if constexpr (std::is_same<Type, IsArray>()) {
      bson_array_builder_append_array_builder_end(_parent.ptr_, _arr->val_);
    } else if constexpr (std::is_same<Type, IsObject>()) {
      bson_append_array_builder_end(_parent.ptr_, _arr->val_);
    } else if constexpr (std::is_same<Type, IsRoot>()) {
      bson_array_builder_build(_arr->val_, doc_);
    } else {
      static_assert(rfl::always_false_v<Type>, "Unsupported type.");
    }
  };
  std::visit(handle, _arr->parent_);
}

void Writer::end_object(OutputObjectType* _obj) const noexcept {
  const auto handle = [&](const auto _parent) {
    using Type = std::remove_cvref_t<decltype(_parent)>;
    if constexpr (std::is_same<Type, IsArray>()) {
      bson_array_builder_append_document_end(_parent.ptr_, _obj->val_);
    } else if constexpr (std::is_same<Type, IsObject>()) {
      bson_append_document_end(_parent.ptr_, _obj->val_);
    } else if constexpr (std::is_same<Type, IsRoot>()) {
    } else {
      static_assert(rfl::always_false_v<Type>, "Unsupported type.");
    }
  };
  std::visit(handle, _obj->parent_);
}

}  // namespace rfl::bson
