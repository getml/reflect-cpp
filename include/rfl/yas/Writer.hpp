#ifndef RFL_YAS_WRITER_HPP_
#define RFL_YAS_WRITER_HPP_

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../Vectorstring.hpp"
#include "../always_false.hpp"
#include "../internal/is_literal.hpp"

namespace rfl::yas {

template <class OArchive>
struct Writer {
  struct OutputArrayType {
    OArchive* ar;
  };

  struct OutputMapType {
    OArchive* ar;
  };

  struct OutputObjectType {
    OArchive* ar;
  };

  struct OutputUnionType {
    OArchive* ar;
  };

  struct OutputVarType {
    OArchive* ar;
  };

  Writer(OArchive* _ar) : ar_(_ar) {}

  template <class T>
  static constexpr bool has_custom_constructor = false;

  OutputArrayType array_as_root(const size_t _size) const noexcept {
    (*ar_) & _size;
    return OutputArrayType{ar_};
  }

  OutputMapType map_as_root(const size_t _size) const noexcept {
    (*ar_) & _size;
    return OutputMapType{ar_};
  }

  OutputObjectType object_as_root(const size_t _size) const noexcept {
    return OutputObjectType{ar_};
  }

  OutputUnionType union_as_root() const noexcept { return OutputUnionType{ar_}; }

  template <class T>
  void value_as_root(const T& _var) const noexcept {
    if constexpr (internal::is_literal_v<T>) {
      (*ar_) & _var.to_string();
    } else {
      (*ar_) & _var;
    }
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept {
    (*_parent->ar) & _size;
    return OutputArrayType{_parent->ar};
  }

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const noexcept {
    (*_parent->ar) & _size;
    return OutputArrayType{_parent->ar};
  }

  OutputMapType add_map_to_array(const size_t _size,
                                 OutputArrayType* _parent) const noexcept {
    (*_parent->ar) & _size;
    return OutputMapType{_parent->ar};
  }

  OutputMapType add_map_to_object(const std::string_view& _name,
                                  const size_t _size,
                                  OutputObjectType* _parent) const noexcept {
    (*_parent->ar) & _size;
    return OutputMapType{_parent->ar};
  }

  OutputObjectType add_object_to_array(
      const size_t _size, OutputArrayType* _parent) const noexcept {
    return OutputObjectType{_parent->ar};
  }

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept {
    return OutputObjectType{_parent->ar};
  }

  OutputUnionType add_union_to_array(OutputArrayType* _parent) const noexcept {
    return OutputUnionType{_parent->ar};
  }

  OutputUnionType add_union_to_object(const std::string_view& _name,
                                      OutputObjectType* _parent) const noexcept {
    return OutputUnionType{_parent->ar};
  }

  template <class T>
  void add_value_to_array(const T& _var,
                          OutputArrayType* _parent) const noexcept {
    if constexpr (internal::is_literal_v<T>) {
      (*_parent->ar) & _var.to_string();
    } else {
      (*_parent->ar) & _var;
    }
  }

  template <class T>
  void add_value_to_object(const std::string_view& _name, const T& _var,
                           OutputObjectType* _parent) const noexcept {
    if constexpr (internal::is_literal_v<T>) {
      (*_parent->ar) & _var.to_string();
    } else {
      (*_parent->ar) & _var;
    }
  }

  template <class T>
  void add_value_to_map(const std::string_view& _name, const T& _var,
                        OutputMapType* _parent) const noexcept {
    (*_parent->ar) & std::string(_name);
    if constexpr (internal::is_literal_v<T>) {
      (*_parent->ar) & _var.to_string();
    } else {
      (*_parent->ar) & _var;
    }
  }

  template <class T>
  void write_union(const size_t _index, const T& _var,
                   OutputUnionType* _parent) const noexcept {
    (*_parent->ar) & _index;
    _var.write(*this, _parent);
  }

  template <class T>
  void write_value(const T& _var, OutputUnionType* _parent) const noexcept {
    if constexpr (internal::is_literal_v<T>) {
      (*_parent->ar) & _var.to_string();
    } else {
      (*_parent->ar) & _var;
    }
  }

  OArchive* ar_;
};

}  // namespace rfl::yas

#endif
