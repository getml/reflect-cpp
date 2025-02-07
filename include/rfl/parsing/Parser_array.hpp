#ifndef RFL_PARSING_PARSER_ARRAY_HPP_
#define RFL_PARSING_PARSER_ARRAY_HPP_

#include <array>
#include <bit>
#include <map>
#include <type_traits>
#include <vector>

#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/ptr_cast.hpp"
#include "ArrayReader.hpp"
#include "Parent.hpp"
#include "Parser_base.hpp"
#include "call_destructors_on_array_where_necessary.hpp"
#include "schema/Type.hpp"

namespace rfl {
namespace parsing {
template <class R, class W, class T, size_t _size, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::array<T, _size>>
struct Parser<R, W, std::array<T, _size>, ProcessorsType> {
 public:
  using InputArrayType = typename R::InputArrayType;
  using InputVarType = typename R::InputVarType;

  using ParentType = Parent<W>;

  static Result<std::array<T, _size>> read(const R& _r,
                                           const InputVarType& _var) noexcept {
    const auto parse =
        [&](const InputArrayType& _arr) -> Result<std::array<T, _size>> {
      alignas(
          std::array<T, _size>) unsigned char buf[sizeof(std::array<T, _size>)];
      auto ptr = internal::ptr_cast<std::array<T, _size>*>(&buf);
      const auto array_reader =
          ArrayReader<R, W, ProcessorsType, T, _size>(&_r, ptr);
      auto err = _r.read_array(array_reader, _arr);
      if (err) {
        call_destructors_on_array_where_necessary(array_reader.num_set(), ptr);
        return error(*err);
      }
      err = array_reader.check_size();
      if (err) {
        call_destructors_on_array_where_necessary(array_reader.num_set(), ptr);
        return error(*err);
      }
      auto result = Result<std::array<T, _size>>(std::move(*ptr));
      call_destructors_on_array_where_necessary(array_reader.num_set(), ptr);
      return result;
    };

    return _r.to_array(_var).and_then(parse);
  }

  template <class P>
  static void write(const W& _w, const std::array<T, _size>& _arr,
                    const P& _parent) noexcept {
    auto arr = ParentType::add_array(_w, _size, _parent);
    const auto new_parent = typename ParentType::Array{&arr};
    for (const auto& e : _arr) {
      Parser<R, W, std::remove_cvref_t<T>, ProcessorsType>::write(_w, e,
                                                                  new_parent);
    }
    _w.end_array(&arr);
  }

  static schema::Type to_schema(
      std::map<std::string, schema::Type>* _definitions) {
    using U = std::remove_cvref_t<T>;
    return schema::Type{schema::Type::FixedSizeTypedArray{
        .size_ = _size,
        .type_ = Ref<schema::Type>::make(
            Parser<R, W, U, ProcessorsType>::to_schema(_definitions))}};
  }
};

}  // namespace parsing
}  // namespace rfl

#endif
