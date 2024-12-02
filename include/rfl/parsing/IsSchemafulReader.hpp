#ifndef RFL_PARSING_ISSCHEMAFULREADER_HPP_
#define RFL_PARSING_ISSCHEMAFULREADER_HPP_

#include <concepts>
#include <string>
#include <variant>

namespace rfl::parsing {

using MockVariantType = std::variant<std::string, int>;

template <class R>
struct MockVariantParser {
  static rfl::Result<MockVariantType> parse(const R&, const size_t,
                                            typename R::InputVarType&) {
    return Error("This is a mock type.");
  }
};

template <class R>
concept IsSchemafulReader =
    requires(R r, typename R::InputVarType var, typename R::InputUnionType u) {
      {
        r.to_union(var)
      } -> std::same_as<rfl::Result<typename R::InputUnionType>>;

      {
        r.template to_variant<MockVariantType, MockVariantParser<R>>(u)
      } -> std::same_as<rfl::Result<MockVariantType>>;
    };

}  // namespace rfl::parsing

#endif
