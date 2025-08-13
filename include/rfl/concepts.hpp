#ifndef RFL_CONCEPTS_HPP_
#define RFL_CONCEPTS_HPP_

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace rfl::concepts {

/// Concept for byte-like types that can be used in contiguous containers
/// Includes char, signed char, unsigned char, std::byte, and uint8_t
template <typename T>
concept ByteLike = std::same_as<T, char> || std::same_as<T, signed char> ||
                   std::same_as<T, unsigned char> ||
                   std::same_as<T, std::uint8_t> || std::same_as<T, std::byte>;

/// Concept for containers with a contiguous sequence of byte-like types
/// Requires:
/// - Container has a value_type that is byte-like
/// - Container provides data() method returning a pointer to contiguous memory
/// - Container provides size() method returning the number of elements
/// - Container supports range-based for loops (begin/end)
template <typename Container>
concept ContiguousByteContainer = requires(const Container& c) {
  typename Container::value_type;
  { c.data() } -> std::convertible_to<const typename Container::value_type*>;
  { c.size() } -> std::convertible_to<std::size_t>;
  { c.begin() } -> std::input_iterator;
  { c.end() } -> std::input_iterator;
  requires ByteLike<typename Container::value_type>;
  requires std::contiguous_iterator<decltype(c.begin())>;
};

/// Concept for mutable containers with a contiguous sequence of byte-like types
/// Extends ContiguousByteContainer with mutable access requirements
template <typename Container>
concept MutableContiguousByteContainer =
    ContiguousByteContainer<Container> && requires(Container& c) {
      { c.data() } -> std::convertible_to<typename Container::value_type*>;
      { c.begin() } -> std::output_iterator<typename Container::value_type>;
      { c.end() } -> std::output_iterator<typename Container::value_type>;
    };

/// Concept for back-insertable byte containers (like std::vector<uint8_t>)
/// Useful for containers that can grow dynamically during serialization
template <typename Container>
concept BackInsertableByteContainer =
    ContiguousByteContainer<Container> &&
    requires(Container& c, typename Container::value_type v) {
      c.push_back(v);
      c.reserve(std::size_t{});
      { c.capacity() } -> std::convertible_to<std::size_t>;
    };

/// Concept for byte spans or views (read-only, non-owning containers)
/// Includes std::span<const uint8_t>, std::string_view when used with char
/// data, etc.
template <typename Container>
concept ByteSpanLike = ContiguousByteContainer<Container> &&
                       std::is_trivially_copyable_v<Container> &&
                       std::is_trivially_destructible_v<Container>;

}  // namespace rfl::concepts

#endif  // RFL_CONCEPTS_HPP_
