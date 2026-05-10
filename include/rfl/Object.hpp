#ifndef RFL_OBJECT_HPP_
#define RFL_OBJECT_HPP_

#include <algorithm>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "Result.hpp"

namespace rfl {

/// Used to embed additional fields for which the names cannot be known in
/// advance and can therefore not be encoded in the struct.
/// This is an ordered associative container that preserves insertion order,
/// unlike std::map. Useful for JSON objects with unknown or dynamic keys.
/// @tparam T The value type stored in the object
template <class T>
class Object {
 public:
  using DataType = std::vector<std::pair<std::string, T>>;
  using Type = T;

  /// We want this to behave as similarly to C++ standard containers as
  /// possible.
  using key_type = std::string;
  using mapped_type = T;
  using value_type = std::pair<std::string, T>;
  using size_type = typename DataType::size_type;
  using difference_type = typename DataType::difference_type;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename DataType::pointer;
  using const_pointer = typename DataType::const_pointer;
  using iterator = typename DataType::iterator;
  using const_iterator = typename DataType::const_iterator;
  using reverse_iterator = typename DataType::reverse_iterator;
  using const_reverse_iterator = typename DataType::const_reverse_iterator;

  /// Default constructor - creates an empty object.
  Object() : data_(), i_(0) {}

  /// Copy constructor.
  /// @param _f The object to copy from
  Object(const Object<T>& _f) = default;

  /// Move constructor.
  /// @param _f The object to move from
  Object(Object<T>&& _f) noexcept = default;

  /// Destructor.
  ~Object() = default;

  /// Iterator to the beginning.
  /// @return Iterator to the first element
  auto begin() { return data_.begin(); }

  /// Iterator to the beginning (const version).
  /// @return Const iterator to the first element
  auto begin() const { return data_.begin(); }

  /// Const iterator to the beginning.
  /// @return Const iterator to the first element
  auto cbegin() const { return data_.cbegin(); }

  /// Iterator to the end.
  /// @return Iterator past the last element
  auto end() { return data_.end(); }

  /// Iterator to the end (const version).
  /// @return Const iterator past the last element
  auto end() const { return data_.end(); }

  /// Const iterator to the end.
  /// @return Const iterator past the last element
  auto cend() const { return data_.cend(); }

  /// Reverse iterator to the beginning (points to last element).
  /// @return Reverse iterator to the last element
  auto rbegin() { return data_.rbegin(); }

  /// Reverse iterator to the beginning (const version).
  /// @return Const reverse iterator to the last element
  auto rbegin() const { return data_.rbegin(); }

  /// Const reverse iterator to the beginning.
  /// @return Const reverse iterator to the last element
  auto crbegin() const { return data_.crbegin(); }

  /// Reverse iterator to the end (points before first element).
  /// @return Reverse iterator before the first element
  auto rend() { return data_.rend(); }

  /// Reverse iterator to the end (const version).
  /// @return Const reverse iterator before the first element
  auto rend() const { return data_.rend(); }

  /// Const reverse iterator to the end.
  /// @return Const reverse iterator before the first element
  auto crend() const { return data_.crend(); }

  /// Copy assignment operator.
  /// @param _f The object to copy from
  /// @return Reference to this object
  Object<T>& operator=(const Object<T>& _f) = default;

  /// Move assignment operator.
  /// @param _f The object to move from
  /// @return Reference to this object
  Object<T>& operator=(Object<T>&& _f) = default;

  /// Whether the object is empty.
  /// @return true if the object contains no elements
  auto empty() const { return data_.size() == 0; }

  /// The number of elements currently inside the object.
  /// @return The number of key-value pairs
  auto size() const { return data_.size(); }

  /// Counts occurrences of a key (0 or 1 since keys should be unique).
  /// @param key The key to count
  /// @return The number of elements with the specified key (typically 0 or 1)
  std::size_t count(const key_type& key) const { return std::count_if(cbegin(), cend(), [&](const auto& p) { return p.first == key; }); }

  /// The maximum possible size of the object.
  /// @return The maximum number of elements that can be stored
  auto max_size() const { return data_.max_size(); }

  /// Inserts new elements at the end (variadic copy version).
  /// @tparam Args Types of the values to insert
  /// @param _values The key-value pairs to insert
  template <class... Args>
  void insert(const Args&... _values) {
    (data_.push_back(_values), ...);
    i_ = 0;
  }

  /// Inserts new elements at the end (variadic move version).
  /// @tparam Args Types of the values to insert
  /// @param _values The key-value pairs to insert (will be moved)
  template <class... Args>
  void insert(Args&&... _values) {
    (data_.emplace_back(std::move(_values)), ...);
    i_ = 0;
  }

  /// Inserts a new key-value pair at the end (copy version).
  /// @param _k The key
  /// @param _v The value to associate with the key
  void insert(const std::string& _k, const T& _v) {
    insert(std::make_pair(_k, _v));
  }

  /// Inserts a new key-value pair at the end (value move version).
  /// @param _k The key
  /// @param _v The value to associate with the key (will be moved)
  void insert(const std::string& _k, T&& _v) {
    insert(std::make_pair(_k, std::move(_v)));
  }

  /// Inserts a new key-value pair at the end (both move version).
  /// @param _k The key (will be moved)
  /// @param _v The value to associate with the key (will be moved)
  void insert(std::string&& _k, T&& _v) {
    insert(std::make_pair(std::move(_k), std::move(_v)));
  }

  /// Inserts a new key-value pair at the end (string_view key, copy value).
  /// @param _k The key as a string_view
  /// @param _v The value to associate with the key
  void insert(const std::string_view& _k, const T& _v) {
    insert(std::make_pair(std::string(_k), _v));
  }

  /// Inserts a new key-value pair at the end (string_view key, move value).
  /// @param _k The key as a string_view
  /// @param _v The value to associate with the key (will be moved)
  void insert(const std::string_view& _k, T&& _v) {
    insert(std::make_pair(std::string(_k), std::move(_v)));
  }

  /// Alias for insert that primarily exists for compatibility with standard containers.
  /// Emplaces elements in-place at the end (move version).
  /// @tparam Args Types of the arguments to forward
  /// @param _args The arguments to forward to insert
  template <class... Args>
  void emplace(Args&&... _args) {
    insert(std::forward<Args>(_args)...);
  }

  /// Alias for insert that primarily exists for compatibility with standard containers.
  /// Emplaces elements in-place at the end (copy version).
  /// @tparam Args Types of the arguments to forward
  /// @param _args The arguments to forward to insert
  template <class... Args>
  void emplace(const Args&... _args) {
    insert(_args...);
  }

  /// Inserts several new elements at the end from an iterator range.
  /// @tparam InputIt Iterator type
  /// @param _first Iterator to the first element to insert
  /// @param _last Iterator past the last element to insert
  template <class InputIt>
  void insert_range(InputIt _first, InputIt _last) {
    for (auto it = _first; it != _last; ++it) {
      insert(*it);
    }
  }

  /// Inserts several new elements at the end from a range.
  /// @tparam RangeType Type of the range
  /// @param _range The range of elements to insert
  template <class RangeType>
  void insert_range(RangeType _range) {
    for (const auto& val : _range) {
      insert(val);
    }
  }

  /// Returns the element signified by the key or creates a new one.
  /// If the key doesn't exist, inserts a default-constructed value.
  /// @param _key The key to look up or create
  /// @return Reference to the value associated with the key
  T& operator[](const std::string& _key) {
    const auto i = find(_key);
    if (i != size()) {
      return data_[i].second;
    }
    data_.emplace_back(std::make_pair(_key, T()));
    i_ = 0;
    return data_.back().second;
  }

  /// Returns the element signified by the key or creates a new one (move version).
  /// If the key doesn't exist, inserts a default-constructed value.
  /// @param _key The key to look up or create (will be moved)
  /// @return Reference to the value associated with the key
  T& operator[](std::string&& _key) {
    const auto i = find(_key);
    if (i != size()) {
      return data_[i].second;
    }
    data_.emplace_back(std::make_pair(std::move(_key), T()));
    i_ = 0;
    return data_.back().second;
  }

  /// Deletes all elements from the object.
  void clear() {
    data_.clear();
    i_ = 0;
  }

  /// Returns the element signified by the key or throws an exception.
  /// @param _key The key to look up
  /// @return Reference to the value associated with the key
  /// @throws std::runtime_error if the key is not found
  T& at(const std::string& _key) {
    const auto i = find(_key);
    if (i == size()) {
      throw std::runtime_error("Key named '" + _key + "' not found.");
    }
    return data_[i].second;
  }

  /// Returns the element signified by the key or throws an exception (const version).
  /// @param _key The key to look up
  /// @return Const reference to the value associated with the key
  /// @throws std::runtime_error if the key is not found
  const T& at(const std::string& _key) const {
    const auto i = find(_key);
    if (i == size()) {
      throw std::runtime_error("Key named '" + _key + "' not found.");
    }
    return data_[i].second;
  }

  /// Returns a result wrapping the element signified by the key.
  /// This is a non-throwing alternative to at().
  /// @param _key The key to look up
  /// @return Result containing the value if found, or an error if not found
  Result<T> get(const std::string& _key) const noexcept {
    const auto i = find(_key);
    if (i == size()) {
      return error("Key named '" + _key + "' not found.");
    }
    return data_[i].second;
  }

 private:
  /// Finds the index of a key in the data vector.
  /// Uses a rotating search pattern starting from i_ for better cache locality.
  /// @param _key The key to find
  /// @return The index of the key, or size() if not found
  size_t find(const std::string& _key) const {
    for (size_t i = i_; i < size(); ++i) {
      if (data_[i].first == _key) {
        i_ = i + 1;
        return i;
      }
    }
    for (size_t i = 0; i < i_; ++i) {
      if (data_[i].first == _key) {
        i_ = i + 1;
        return i;
      }
    }
    return size();
  }

 private:
  /// The underlying data storage - vector of key-value pairs
  DataType data_;

  /// Allows faster access by remembering the last accessed position
  mutable size_t i_;
};

}  // namespace rfl

#endif
