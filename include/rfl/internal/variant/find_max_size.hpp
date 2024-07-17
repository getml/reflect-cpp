#ifndef RFL_INTERNAL_VARIANT_FIND_MAX_SIZE_HPP_
#define RFL_INTERNAL_VARIANT_FIND_MAX_SIZE_HPP_

namespace rfl::internal::variant {

template <class T>
struct SizeWrapper {
  using Type = T;
  static constexpr unsigned long size_ = sizeof(T);
};

template <class T1, class T2>
consteval auto operator|(const SizeWrapper<T1>& _s1,
                         const SizeWrapper<T2>& _s2) {
  if constexpr (sizeof(T2) > sizeof(T1)) {
    return _s2;
  } else {
    return _s1;
  }
}

template <class Head, class... Tail>
consteval auto find_max_size() {
  return (SizeWrapper<Head>{} | ... | SizeWrapper<Tail>{});
}

}  // namespace rfl::internal::variant

#endif
