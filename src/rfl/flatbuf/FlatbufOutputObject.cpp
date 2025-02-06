/*

MIT License

Copyright (c) 2023-2024 Code17 GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "rfl/flatbuf/FlatbufOutputObject.hpp"

#include <type_traits>

namespace rfl::flatbuf {

FlatbufOutputObject::FlatbufOutputObject(const schema::Type::Table& _schema,
                                         FlatbufOutputParent* _parent,
                                         flatbuffers::FlatBufferBuilder* _fbb)
    : schema_(_schema), parent_(_parent), fbb_(_fbb) {
  offsets_.push_back(0);
}

void FlatbufOutputObject::add_offset(const flatbuffers::uoffset_t _offset) {
  auto offset = flatbuffers::Offset<>(_offset);
  const auto ptr = internal::ptr_cast<const uint8_t*>(&offset);
  data_.insert(data_.end(), ptr, ptr + sizeof(flatbuffers::Offset<>));
  offsets_.push_back(offsets_.back() + sizeof(flatbuffers::Offset<>));
}

void FlatbufOutputObject::end() {
  const auto start = fbb_->StartTable();
  for (size_t i = 0; i < offsets_.size() - 1; ++i) {
    fbb_->AddOffset<>(calc_vtable_offset(i),
                      *internal::ptr_cast<flatbuffers::Offset<>*>(data_.data() +
                                                                  offsets_[i]));
  }
  auto offset = fbb_->EndTable(start);
  if (parent_) {
    parent_->add_offset(offset);
  } else {
    fbb_->Finish(flatbuffers::Offset<>(offset));
  }
}

}  // namespace rfl::flatbuf
