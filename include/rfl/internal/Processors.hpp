#ifndef RFL_INTERNAL_PROCESSORS_HPP_
#define RFL_INTERNAL_PROCESSORS_HPP_

namespace rfl::internal {

template <class... Ps>
struct Processors;

template <>
struct Processors<> {
  template <class T>
  static auto apply_all(auto&& _named_tuple) {
    return _named_tuple;
  }
};

template <class Head, class... Tail>
struct Processors<Head, Tail...> {
  template <class T>
  static auto apply_all(auto&& _named_tuple) {
    return Processors<Tail...>::template apply_all<T>(
        Head::template process<T>(std::move(_named_tuple)));
  }
};

}  // namespace rfl::internal

#endif
