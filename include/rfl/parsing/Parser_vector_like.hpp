#ifndef RFL_PARSING_PARSER_VECTOR_LIKE_HPP_
#define RFL_PARSING_PARSER_VECTOR_LIKE_HPP_

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../Result.hpp"
#include "../always_false.hpp"
#include "Parser_base.hpp"
#include "VectorParser.hpp"

namespace rfl {
namespace parsing {

/**
 * @brief Parser specialization for std::deque.
 */
template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::deque<T>>
struct Parser<R, W, std::deque<T>, ProcessorsType>
    : public VectorParser<R, W, std::deque<T>, ProcessorsType> {};

/**
 * @brief Parser specialization for std::forward_list.
 */
template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::forward_list<T>>
struct Parser<R, W, std::forward_list<T>, ProcessorsType>
    : public VectorParser<R, W, std::forward_list<T>, ProcessorsType> {};

/**
 * @brief Parser specialization for std::list.
 */
template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::list<T>>
struct Parser<R, W, std::list<T>, ProcessorsType>
    : public VectorParser<R, W, std::list<T>, ProcessorsType> {};

/**
 * @brief Parser specialization for std::map.
 */
template <class R, class W, class K, class V, class Compare, class Allocator,
          class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::map<K, V, Compare, Allocator>>
struct Parser<R, W, std::map<K, V, Compare, Allocator>, ProcessorsType>
    : public VectorParser<R, W, std::map<K, V, Compare, Allocator>,
                          ProcessorsType> {};

/**
 * @brief Parser specialization for std::multimap.
 */
template <class R, class W, class K, class V, class Compare, class Allocator,
          class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::multimap<K, V, Compare, Allocator>>
struct Parser<R, W, std::multimap<K, V, Compare, Allocator>, ProcessorsType>
    : public VectorParser<R, W, std::multimap<K, V, Compare, Allocator>,
                          ProcessorsType> {};

/**
 * @brief Parser specialization for std::multiset.
 */
template <class R, class W, class T, class Compare, class Allocator,
          class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::multiset<T, Compare, Allocator>>
struct Parser<R, W, std::multiset<T, Compare, Allocator>, ProcessorsType>
    : public VectorParser<R, W, std::multiset<T, Compare, Allocator>,
                          ProcessorsType> {};

/**
 * @brief Parser specialization for std::set.
 */
template <class R, class W, class T, class Compare, class Allocator,
          class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::set<T, Compare, Allocator>>
struct Parser<R, W, std::set<T, Compare, Allocator>, ProcessorsType>
    : public VectorParser<R, W, std::set<T, Compare, Allocator>,
                          ProcessorsType> {};

/**
 * @brief Parser specialization for std::unordered_map.
 */
template <class R, class W, class K, class V, class Hash, class KeyEqual,
          class Allocator, class ProcessorsType>
  requires AreReaderAndWriter<
      R, W, std::unordered_map<K, V, Hash, KeyEqual, Allocator>>
struct Parser<R, W, std::unordered_map<K, V, Hash, KeyEqual, Allocator>,
              ProcessorsType>
    : public VectorParser<R, W,
                          std::unordered_map<K, V, Hash, KeyEqual, Allocator>,
                          ProcessorsType> {};

/**
 * @brief Parser specialization for std::unordered_multiset.
 */
template <class R, class W, class T, class Hash, class KeyEqual,
          class Allocator, class ProcessorsType>
  requires AreReaderAndWriter<
      R, W, std::unordered_multiset<T, Hash, KeyEqual, Allocator>>
struct Parser<R, W, std::unordered_multiset<T, Hash, KeyEqual, Allocator>,
              ProcessorsType>
    : public VectorParser<R, W,
                          std::unordered_multiset<T, Hash, KeyEqual, Allocator>,
                          ProcessorsType> {};

/**
 * @brief Parser specialization for std::unordered_multimap.
 */
template <class R, class W, class K, class V, class Hash, class KeyEqual,
          class Allocator, class ProcessorsType>
  requires AreReaderAndWriter<
      R, W, std::unordered_multimap<K, V, Hash, KeyEqual, Allocator>>
struct Parser<R, W, std::unordered_multimap<K, V, Hash, KeyEqual, Allocator>,
              ProcessorsType>
    : public VectorParser<R, W, std::unordered_multimap<K, V>, ProcessorsType> {
};

/**
 * @brief Parser specialization for std::unordered_set.
 */
template <class R, class W, class T, class Hash, class KeyEqual,
          class Allocator, class ProcessorsType>
  requires AreReaderAndWriter<R, W,
                              std::unordered_set<T, Hash, KeyEqual, Allocator>>
struct Parser<R, W, std::unordered_set<T, Hash, KeyEqual, Allocator>,
              ProcessorsType>
    : public VectorParser<R, W,
                          std::unordered_set<T, Hash, KeyEqual, Allocator>,
                          ProcessorsType> {};

/**
 * @brief Parser specialization for std::vector.
 */
template <class R, class W, class T, class ProcessorsType>
  requires AreReaderAndWriter<R, W, std::vector<T>>
struct Parser<R, W, std::vector<T>, ProcessorsType>
    : public VectorParser<R, W, std::vector<T>, ProcessorsType> {};

}  // namespace parsing
}  // namespace rfl

#endif
