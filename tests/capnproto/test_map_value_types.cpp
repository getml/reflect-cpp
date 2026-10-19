#include <cstdint>
#include <limits>
#include <map>
#include <optional>
#include <rfl.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "write_and_read.hpp"

namespace test_map_value_types {

struct PersonWithStringMap {
  rfl::Rename<"firstName", std::string> first_name;
  std::map<std::string, std::string> nicknames;
};

TEST(capnproto, test_map_with_string_values) {
  auto nicknames = std::map<std::string, std::string>();
  nicknames["Bart"] = "El Barto";
  nicknames["Lisa"] = "Lis";
  nicknames["Maggie"] = "Mags";

  const auto homer = PersonWithStringMap{.first_name = "Homer",
                                         .nicknames = std::move(nicknames)};

  write_and_read(homer);
}

template <class Map>
struct MapValues {
  Map values;
};

template <class Map>
void check_map_values(const Map& _values) {
  using T = MapValues<Map>;
  const auto serialized = rfl::capnproto::write(T{.values = _values});
  const auto restored = rfl::capnproto::read<T>(serialized);
  ASSERT_TRUE(restored) << restored.error().what();
  EXPECT_EQ(restored.value().values, _values);
}

TEST(capnproto, test_map_with_integer_values) {
  check_map_values(std::map<std::string, int>{});
  check_map_values(std::map<std::string, int>{
      {"negative", -42}, {"zero", 0}, {"positive", 42}});
  check_map_values(std::map<std::string, unsigned int>{
      {"zero", 0}, {"maximum", std::numeric_limits<unsigned int>::max()}});
  check_map_values(std::unordered_map<std::string, std::int64_t>{});
  check_map_values(std::unordered_map<std::string, std::int64_t>{
      {"minimum", std::numeric_limits<std::int64_t>::min()},
      {"large", std::int64_t{1} << 40},
      {"maximum", std::numeric_limits<std::int64_t>::max()}});
  check_map_values(std::map<std::string, std::uint64_t>{
      {"zero", 0}, {"maximum", std::numeric_limits<std::uint64_t>::max()}});
}

TEST(capnproto, test_map_with_bool_and_floating_point_values) {
  check_map_values(
      std::map<std::string, bool>{{"false", false}, {"true", true}});
  check_map_values(
      std::map<std::string, float>{{"negative", -1.25f}, {"positive", 2.5f}});
  check_map_values(std::unordered_map<std::string, double>{{"negative", -1.25},
                                                           {"positive", 2.5}});
}

enum class Color { red, green, blue };

TEST(capnproto, test_map_with_enum_values) {
  check_map_values(std::map<std::string, Color>{{"first", Color::red},
                                                {"second", Color::blue}});
  using Literal = rfl::Literal<"red", "green", "blue">;
  check_map_values(std::unordered_map<std::string, Literal>{
      {"first", Literal::make<"red">()}, {"second", Literal::make<"blue">()}});
}

struct MixedMaps {
  std::map<std::string, int> integers;
  std::map<std::string, bool> booleans;
  std::map<std::string, Color> colors;
  std::map<std::string, std::string> strings;
  std::map<std::string, std::vector<int>> lists;
  std::map<std::string, std::optional<int>> optionals;
  std::map<std::string, std::map<std::string, int>> nested;

  bool operator==(const MixedMaps&) const = default;
};

TEST(capnproto, test_mixed_and_nested_map_value_types) {
  const auto original =
      MixedMaps{.integers = {{"negative", -42}},
                .booleans = {{"true", true}},
                .colors = {{"red", Color::red}},
                .strings = {{"first", "hello"}},
                .lists = {{"first", {1, 2, 3}}},
                .optionals = {{"present", 42}, {"absent", std::nullopt}},
                .nested = {{"first", {{"second", 42}}}}};
  const auto serialized = rfl::capnproto::write(original);
  const auto restored = rfl::capnproto::read<MixedMaps>(serialized);
  ASSERT_TRUE(restored) << restored.error().what();
  EXPECT_EQ(restored.value(), original);
}
}  // namespace test_map_value_types
