#ifndef RFL_INTERNAL_VARIANT_FIND_MAX_SIZE_HPP_
#define RFL_INTERNAL_VARIANT_FIND_MAX_SIZE_HPP_

namespace rfl::internal::variant {

struct SizeWrapper {
  unsigned long size_;
};

constexpr auto wrap_size(const unsigned long _size) {
  return SizeWrapper{_size};
}

constexpr auto operator|(const SizeWrapper& _s1, const SizeWrapper& _s2) {
  return _s2.size_ > _s1.size_ ? _s2 : _s1;
}

template <class Head, class... Tail>
consteval unsigned long find_max_size() {
  return (wrap_size(sizeof(Head)) | ... | wrap_size(sizeof(Tail))).size_;
}

}  // namespace rfl::internal::variant

#endif
