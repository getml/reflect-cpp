#ifndef RFL_PARSING_APPLY_PROCESSORS_HPP_
#define RFL_PARSING_APPLY_PROCESSORS_HPP_

namespace rfl::internal {

template <class T>
auto apply_processors(auto&& _named_tuple) {
  return _named_tuple;
}

template <class T, class Head, class... Tail>
auto apply_processors(auto&& _named_tuple) {
  return apply_processors<T, Tail...>(
      Head::template process<T>(std::move(_named_tuple)));
}

}  // namespace rfl::internal

#endif
