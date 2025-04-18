#ifndef RFL_PARQUET_WRITER_HPP_
#define RFL_PARQUET_WRITER_HPP_

#include <parquet.h>

#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "../Box.hpp"
#include "../Bytestring.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"

namespace rfl::parquet {

class Writer {
 public:
  using OutputArrayType = MsgpackOutputArray;
  using OutputObjectType = MsgpackOutputObject;
  using OutputVarType = MsgpackOutputVar;

  Writer();

  ~Writer();

  OutputArrayType array_as_root(const size_t _size) const noexcept;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const noexcept;

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept;

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept;

  void end_array(OutputArrayType* _arr) const noexcept;

  void end_object(OutputObjectType* _obj) const noexcept;

 private:
};

}  // namespace rfl::parquet

#endif
