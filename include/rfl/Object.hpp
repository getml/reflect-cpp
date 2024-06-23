#ifndef RFL_OBJECT_HPP_
#define RFL_OBJECT_HPP_

#include <algorithm>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace rfl {

/// Used to embed additional fields for which the names cannot be known in
/// advance and can therefore not be encoded in the struct.
template <class T>
class Object {
 public:
  using DataType = std::vector<std::pair<std::string, T>>;

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

  Object() : data_() {}

  Object(const Object<T>& _f) = default;

  Object(Object<T>&& _f) noexcept = default;

  ~Object() = default;

  auto begin() { return data_.begin(); }

  auto begin() const { return data_.begin(); }

  auto cbegin() const { return data_.cbegin(); }

  auto end() { return data_.end(); }

  auto end() const { return data_.end(); }

  auto cend() const { return data_.cend(); }

  auto rbegin() { return data_.rbegin(); }

  auto rbegin() const { return data_.rbegin(); }

  auto crbegin() const { return data_.crbegin(); }

  auto rend() { return data_.rend(); }

  auto rend() const { return data_.rend(); }

  auto crend() const { return data_.rend(); }

  Object<T>& operator=(const Object<T>& _f) = default;

  Object<T>& operator=(Object<T>&& _f) = default;

  auto empty() const { return data_.size() == 0; }

  auto size() const { return data_.size(); }

  auto max_size() const { return data_.max_size(); }

  void insert(const value_type& _value) { data_.push_back(_value); }

  void insert(value_type&& _value) { data_.emplace_back(std::move(_value)); }

  template <class InputIt>
  void insert(InputIt _first, InputIt _last) {
    for (auto it = _first; it != _last; ++it) {
      insert(*it);
    }
  }

  void insert(const std::string& _k, const T& _v) {
    insert(std::make_pair(_k, _v));
  }

  void insert(const std::string& _k, T&& _v) {
    insert(std::make_pair(_k, std::move(_v)));
  }

  void emplace(std::string&& _k, T&& _v) {
    data_.emplace_back(std::make_pair(std::move(_k), std::move(_v)));
  }

  void emplace(std::pair<std::string, T>&& _value) {
    data_.emplace_back(std::move(_value));
  }

  T& operator[](const std::string& _key) {
    for (auto it = begin(); it != end(); ++it) {
      if (it->first == _key) {
        return it->second;
      }
    }
    data_.emplace_back(std::make_pair(_key, T()));
    return data_.back().second;
  }

  T& operator[](std::string&& _key) {
    for (auto it = begin(); it != end(); ++it) {
      if (it->first == _key) {
        return it->second;
      }
    }
    data_.emplace_back(std::make_pair(std::move(_key), T()));
    return data_.back().second;
  }

  void clear() { data_.clear(); }

 private:
  DataType data_;
};

}  // namespace rfl

#endif
