#ifndef RFL_PARSING_SUPPORTSCOMMENTS_HPP_
#define RFL_PARSING_SUPPORTSCOMMENTS_HPP_

#include <concepts>
#include <optional>
#include <string>
#include <string_view>

#include "../Result.hpp"
#include "IsReader.hpp"
#include "IsWriter.hpp"

namespace rfl::parsing {

/// Determines whether a reader supports comments.
template <class R>
concept reader_supports_comments =
    requires(R r, typename R::InputVarType var, std::string_view comment) {
      { r.get_comment(var) } -> std::same_as<std::optional<std::string>>;
    };

/// Determines whether a writer supports comments.
template <class W>
concept writer_supports_comments =
    requires(W w, Parent<W> parent, std::string_view comment) {
      {
        w.add_comment(parent, comment)
      } -> std::same_as<typename W::OutputVarType>;
    };

template <class RorW>
concept supports_comments =
    (IsReader<RorW, int> && reader_supports_comments<RorW>) ||
    (IsWriter<RorW, int> && writer_supports_comments<RorW>);

}  // namespace rfl::parsing

#endif
