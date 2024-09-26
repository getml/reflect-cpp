#ifndef RFL_PARSING_PARSER_MAP_LIKE_HPP_
#define RFL_PARSING_PARSER_MAP_LIKE_HPP_

#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "../ExtraFields.hpp"
#include "../Object.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "MapParser.hpp"
#include "Parser_base.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T, class ProcessorsType>
requires AreReaderAndWriter<R, W, std::map<std::string, T>>
struct Parser<R, W, std::map<std::string, T>, ProcessorsType>
    : public MapParser<R, W, std::map<std::string, T>, ProcessorsType> {
};

template <class R, class W, class T, class ProcessorsType>
requires AreReaderAndWriter<R, W, std::unordered_map<std::string, T>>
struct Parser<R, W, std::unordered_map<std::string, T>, ProcessorsType>
    : public MapParser<R, W, std::unordered_map<std::string, T>,
                       ProcessorsType> {
};

template <class R, class W, class T, class ProcessorsType>
requires AreReaderAndWriter<R, W, Object<T>>
struct Parser<R, W, Object<T>, ProcessorsType>
    : public MapParser<R, W, Object<T>, ProcessorsType> {
};

template <class R, class W, class T, class ProcessorsType>
requires AreReaderAndWriter<R, W, ExtraFields<T>>
struct Parser<R, W, ExtraFields<T>, ProcessorsType>
    : public MapParser<R, W, ExtraFields<T>, ProcessorsType> {
};

}  // namespace parsing
}  // namespace rfl

#endif
