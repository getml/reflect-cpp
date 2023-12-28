#ifndef RFL_PARSING_PARSER_VECTOR_LIKE_HPP_
#define RFL_PARSING_PARSER_VECTOR_LIKE_HPP_

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"
#include "VectorParser.hpp"

namespace rfl {
namespace parsing {

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::deque<T>>
struct Parser<R, W, std::deque<T>> : public VectorParser<R, W, std::deque<T>> {
};

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::forward_list<T>>
struct Parser<R, W, std::forward_list<T>>
    : public VectorParser<R, W, std::forward_list<T>> {
};

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::list<T>>
struct Parser<R, W, std::list<T>> : public VectorParser<R, W, std::list<T>> {
};

template <class R, class W, class K, class V>
requires AreReaderAndWriter<R, W, std::map<K, V>>
struct Parser<R, W, std::map<K, V>>
    : public VectorParser<R, W, std::map<K, V>> {
};

template <class R, class W, class K, class V>
requires AreReaderAndWriter<R, W, std::multimap<K, V>>
struct Parser<R, W, std::multimap<K, V>>
    : public VectorParser<R, W, std::multimap<K, V>> {
};

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::multiset<T>>
struct Parser<R, W, std::multiset<T>>
    : public VectorParser<R, W, std::multiset<T>> {
};

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::set<T>>
struct Parser<R, W, std::set<T>> : public VectorParser<R, W, std::set<T>> {
};

template <class R, class W, class K, class V>
requires AreReaderAndWriter<R, W, std::unordered_map<K, V>>
struct Parser<R, W, std::unordered_map<K, V>>
    : public VectorParser<R, W, std::unordered_map<K, V>> {
};

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::unordered_multiset<T>>
struct Parser<R, W, std::unordered_multiset<T>>
    : public VectorParser<R, W, std::unordered_multiset<T>> {
};

template <class R, class W, class K, class V>
requires AreReaderAndWriter<R, W, std::unordered_multimap<K, V>>
struct Parser<R, W, std::unordered_multimap<K, V>>
    : public VectorParser<R, W, std::unordered_multimap<K, V>> {
};

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::unordered_set<T>>
struct Parser<R, W, std::unordered_set<T>>
    : public VectorParser<R, W, std::unordered_set<T>> {
};

template <class R, class W, class T>
requires AreReaderAndWriter<R, W, std::vector<T>>
struct Parser<R, W, std::vector<T>>
    : public VectorParser<R, W, std::vector<T>> {
};

}  // namespace parsing
}  // namespace rfl

#endif
