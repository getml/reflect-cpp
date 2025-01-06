#ifndef RFL_CAPNPROTO_IS_NAMED_TYPE_HPP_
#define RFL_CAPNPROTO_IS_NAMED_TYPE_HPP_

namespace rfl::capnproto {

inline bool is_named_type(const parsing::schema::Type& _type) {
  return _type.variant_.visit([&](const auto& _v) -> bool {
    using T = std::remove_cvref_t<decltype(_v)>;
    return std::is_same<T, parsing::schema::Type::Object>() ||
           std::is_same<T, parsing::schema::Type::AnyOf>() ||
           std::is_same<T, parsing::schema::Type::StringMap>() ||
           std::is_same<T, parsing::schema::Type::Literal>();
  });
}

}  // namespace rfl::capnproto

#endif

