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
  using difference_type = typename DataType::size_type;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename DataType::pointer;
  using const_pointer = typename DataType::const_pointer;
  using iterator = typename DataType::iterator;
  using const_iterator = typename DataType::const_iterator;
  using reverse_iterator = typename DataType::reverse_iterator;
  using const_reverse_iterator = typename DataType::const_reverse_iterator;

  Object() : data_(), i_(0) {}

  Object(const Object<T>& _f) = default;

  Object(Object<T>&& _f) noexcept = default;

  ~Object() = default;

  /// Iterator to the beginning.
  auto begin() { return data_.begin(); }

  /// Iterator to the beginning.
  auto begin() const { return data_.begin(); }

  /// Const iterator to the beginning.
  auto cbegin() const { return data_.cbegin(); }

  /// Iterator to the end.
  auto end() { return data_.end(); }

  /// Iterator to the end.
  auto end() const { return data_.end(); }

  /// Const iterator to the end.
  auto cend() const { return data_.cend(); }

  /// Reverse iterator.
  auto rbegin() { return data_.rbegin(); }

  /// Reverse iterator.
  auto rbegin() const { return data_.rbegin(); }

  /// Const reverse iterator.
  auto crbegin() const { return data_.crbegin(); }

  /// Reverse iterator.
  auto rend() { return data_.rend(); }

  /// Reverse iterator.
  auto rend() const { return data_.rend(); }

  /// Const reverse iterator.
  auto crend() const { return data_.crend(); }

  Object<T>& operator=(const Object<T>& _f) = default;

  Object<T>& operator=(Object<T>&& _f) = default;

  /// Whether the object is empty.
  auto empty() const { return data_.size() == 0; }

  /// The number of elements currently inside the object.
  auto size() const { return data_.size(); }

  /// The maximum possible size.
  auto max_size() const { return data_.max_size(); }

  /// Inserts a new element at the end.
  void insert(const value_type& _value) {
    data_.push_back(_value);
    i_ = 0;
  }

  /// Inserts a new element at the end.
  void insert(value_type&& _value) {
    data_.emplace_back(std::move(_value));
    i_ = 0;
  }

  /// Inserts several new elements at the end.
  template <class InputIt>
  void insert(InputIt _first, InputIt _last) {
    for (auto it = _first; it != _last; ++it) {
      insert(*it);
    }
  }

  /// Inserts a new element at the end.
  void insert(const std::string& _k, const T& _v) {
    insert(std::make_pair(_k, _v));
  }

  /// Inserts a new element at the end.
  void insert(const std::string& _k, T&& _v) {
    insert(std::make_pair(_k, std::move(_v)));
  }

  /// Inserts a new element at the end.
  void insert(std::string&& _k, T&& _v) {
    insert(std::make_pair(std::move(_k), std::move(_v)));
  }

  /// Inserts a new element at the end.
  void insert(const std::string_view& _k, const T& _v) {
    insert(std::make_pair(std::string(_k), _v));
  }

  /// Inserts a new element at the end.
  void insert(const std::string_view& _k, T&& _v) {
    insert(std::make_pair(std::string(_k), std::move(_v)));
  }

  /// Alias for insert that primarily exists for compatability with standard
  /// containers.
  template <class... Args>
  void emplace(Args&&... _args) {
    insert(std::forward<Args>(_args)...);
  }

  /// Alias for insert that primarily exists for compatability with standard
  /// containers.
  template <class... Args>
  void emplace(const Args&... _args) {
    insert(_args...);
  }

  /// Returns the element signified by the key or creates a new one.
  T& operator[](const std::string& _key) {
    const auto i = find(_key);
    if (i != size()) {
      return data_[i].second;
    }
    data_.emplace_back(std::make_pair(_key, T()));
    i_ = 0;
    return data_.back().second;
  }

  /// Returns the element signified by the key or creates a new one.
  T& operator[](std::string&& _key) {
    const auto i = find(_key);
    if (i != size()) {
      return data_[i].second;
    }
    data_.emplace_back(std::make_pair(std::move(_key), T()));
    i_ = 0;
    return data_.back().second;
  }

  /// Deletes all elements.
  void clear() {
    data_.clear();
    i_ = 0;
  }

  /// Returns the element signified by the key or throws an exception.
  T& at(const std::string& _key) {
    const auto i = find(_key);
    if (i == size()) {
      throw std::runtime_error("Key named '" + _key + "' not found.");
    }
    return data_[i].second;
  }

  /// Returns the element signified by the key or throws an exception.
  const T& at(const std::string& _key) const {
    const auto i = find(_key);
    if (i == size()) {
      throw std::runtime_error("Key named '" + _key + "' not found.");
    }
    return data_[i].second;
  }

  /// Returns a result wrapping the element signified by the key.
  Result<T> get(const std::string& _key) const noexcept {
    const auto i = find(_key);
    if (i == size()) {
      return error("Key named '" + _key + "' not found.");
    }
    return data_[i].second;
  }

 private:
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
  DataType data_;

  /// Allows faster access
  mutable size_t i_;
};

}  // namespace rfl

#endif
