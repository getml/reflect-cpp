#ifndef RFL_PARSING_SCHEMAFUL_ISSCHEMAFULREADER_HPP_
#define RFL_PARSING_SCHEMAFUL_ISSCHEMAFULREADER_HPP_

#include <concepts>
#include <string>
#include <variant>

namespace rfl::parsing::schemaful {

using MockVariantType = std::variant<std::string, int>;

template <class R>
struct MockMapReader {
  void read(const std::string_view&, typename R::InputVarType&) const {}
};

template <class R>
struct MockObjectReader {
  void read(const int, typename R::InputVarType&) const {}
};

template <class R>
struct MockUnionReader {
  static rfl::Result<MockVariantType> read(const R&, const size_t,
                                           typename R::InputVarType&) {
    return Error("This is a mock type.");
  }
};

template <class R>
concept IsSchemafulReader =
    requires(R r, typename R::InputVarType var, typename R::InputObjectType obj,
             typename R::InputMapType m, typename R::InputUnionType u,
             MockMapReader<R> map_reader, MockObjectReader<R> object_reader) {
      { r.read_map(map_reader, m) } -> std::same_as<std::optional<Error>>;

      {
        r.read_object(object_reader, obj)
      } -> std::same_as<std::optional<Error>>;

      {
        r.template read_union<MockVariantType, MockUnionReader<R>>(u)
      } -> std::same_as<rfl::Result<MockVariantType>>;

      { r.to_map(var) } -> std::same_as<rfl::Result<typename R::InputMapType>>;

      {
        r.to_union(var)
      } -> std::same_as<rfl::Result<typename R::InputUnionType>>;
    };

}  // namespace rfl::parsing::schemaful

#endif
