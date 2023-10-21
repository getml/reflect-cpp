#ifndef RFL_PARSING_ISREADER_HPP_
#define RFL_PARSING_ISREADER_HPP_

#include <concepts>
#include <unordered_map>

#include "rfl/Result.hpp"

namespace rfl {
namespace parsing {

template <class R, class T>
concept IsReader = requires(R r, typename R::InputArrayType arr,
                            typename R::InputObjectType obj,
                            typename R::InputVarType var) {
    { r.template to_basic_type<T>(&var) } -> std::same_as<rfl::Result<T>>;

    {
        r.to_array(&var)
        } -> std::same_as<rfl::Result<typename R::InputArrayType>>;

    {
        r.to_map(&obj)
        } -> std::same_as<
            std::vector<std::pair<std::string, typename R::InputVarType>>>;

    {
        r.to_object(&var)
        } -> std::same_as<rfl::Result<typename R::InputObjectType>>;

    { r.to_vec(&arr) } -> std::same_as<std::vector<typename R::InputVarType>>;

    {
        r.template use_custom_constructor<T>(&var)
        } -> std::same_as<rfl::Result<T>>;
};

}  // namespace parsing
}  // namespace rfl

#endif
