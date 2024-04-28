#include <iostream>
#include <rfl.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_monster_example {

using Color = rfl::Literal<"Red", "Green", "Blue">;

struct Weapon {
  std::string name;
  short damage;
};

using Equipment = rfl::Variant<rfl::Field<"weapon", Weapon>>;

struct Vec3 {
  float x;
  float y;
  float z;
};

struct Monster {
  Vec3 pos;
  short mana = 150;
  short hp = 100;
  std::string name;
  bool friendly = false;
  std::vector<int> inventory;
  Color color = Color::make<"Blue">();
  std::vector<Weapon> weapons;
  Equipment equipped;
  std::vector<Vec3> path;
};

TEST(bson, test_monster_example) {
  const auto sword = Weapon{.name = "Sword", .damage = 3};
  const auto axe = Weapon{.name = "Axe", .damage = 5};

  const auto weapons = std::vector<Weapon>({sword, axe});

  const auto position = Vec3{1.0f, 2.0f, 3.0f};

  const auto inventory = std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  const auto orc = Monster{.pos = position,
                           .mana = 150,
                           .hp = 80,
                           .name = "MyMonster",
                           .inventory = inventory,
                           .color = Color::make<"Red">(),
                           .weapons = weapons,
                           .equipped = rfl::make_field<"weapon">(axe)};

  write_and_read(orc);
}
}  // namespace test_monster_example
