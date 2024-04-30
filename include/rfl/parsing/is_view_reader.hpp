#ifndef RFL_PARSING_ISVIEWREADER_HPP_
#define RFL_PARSING_ISVIEWREADER_HPP_

#include "ViewReader.hpp"

namespace rfl ::parsing {

template <class T>
class is_view_reader;

template <class T>
class is_view_reader : public std::false_type {};

template <class R, class W, class NamedTupleType, class ProcessorsType>
class is_view_reader<ViewReader<R, W, NamedTupleType, ProcessorsType>>
    : public std::true_type {};

template <class T>
constexpr bool is_view_reader_v =
    is_view_reader<std::remove_cvref_t<std::remove_pointer_t<T>>>::value;

}  // namespace rfl::parsing

#endif
