#ifndef RFL_PARSING_PARSER_MAP_LIKE_HPP_
#define RFL_PARSING_PARSER_MAP_LIKE_HPP_

#include <map>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

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

}  // namespace parsing
}  // namespace rfl

#endif
