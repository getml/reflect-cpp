#ifndef RFL_CAPNPROTO_GET_ROOT_NAME_HPP_
#define RFL_CAPNPROTO_GET_ROOT_NAME_HPP_

#include <string>

#include "../Processors.hpp"
#include "../internal/has_custom_parser.hpp"
#include "../internal/has_reflection_type_v.hpp"
#include "../parsing/make_type_name.hpp"
#include "Parser.hpp"

namespace rfl::capnproto {

template <class T, class... Ps>
inline std::string get_root_name() {
  if constexpr (internal::has_reflection_type_v<T>) {
    return get_root_name<std::remove_cvref_t<typename T::ReflectionType>>();
  } else if constexpr (internal::has_custom_parser<Reader, Writer, T,
                                                   Processors<Ps...>>) {
    return get_root_name<
        typename Parser<T, Processors<Ps...>>::CustomParserHelperStruct>();
  } else {
    return internal::strings::to_pascal_case(parsing::make_type_name<T>());
  }
}

}  // namespace rfl::capnproto

#endif
