#ifndef RFL_PARSING_SUPPORTSCOMMENTS_HPP_
#define RFL_PARSING_SUPPORTSCOMMENTS_HPP_

#include <concepts>
#include <optional>
#include <string>
#include <string_view>

#include "../Result.hpp"

namespace rfl::parsing {

/// Determines whether a writer supports comments.
template <class W>
concept supports_comments =
    requires(W w, typename W::OutputArrayType arr,
             typename W::OutputObjectType obj, std::string_view comment) {
      { w.add_comment_to_array(comment, &arr) } -> std::same_as<void>;

      { w.add_comment_to_object(comment, &obj) } -> std::same_as<void>;
    };

}  // namespace rfl::parsing

#endif
