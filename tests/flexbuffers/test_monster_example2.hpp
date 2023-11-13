#include <iostream>
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

void test_monster_example2() {
  std::cout << "test_monster_example2" << std::endl;

  using Color = rfl::Literal<"Red", "Green", "Blue">;

  struct Weapon {
    rfl::Field<"name", std::string> name;
    rfl::Field<"damage", short> damage;
  };

  using Equipment = rfl::Variant<rfl::Field<"weapon", Weapon>>;

  struct Vec3 {
    rfl::Field<"x", float> x;
    rfl::Field<"y", float> y;
    rfl::Field<"z", float> z;
  };

  struct Monster {
    rfl::Field<"pos", Vec3> pos;
    rfl::Field<"mana", short> mana = 150;
    rfl::Field<"hp", short> hp = 100;
    rfl::Field<"name", std::string> name;
    rfl::Field<"friendly", bool> friendly = false;
    rfl::Field<"inventory", std::vector<std::uint8_t>> inventory;
    rfl::Field<"color", Color> color = Color::make<"Blue">();
    rfl::Field<"weapons", std::vector<Weapon>> weapons;
    rfl::Field<"equipped", Equipment> equipped;
    rfl::Field<"path", std::vector<Vec3>> path;
  };

  const auto sword = Weapon{.name = "Sword", .damage = 3};
  const auto axe = Weapon{.name = "Axe", .damage = 5};

  const auto weapons = std::vector<Weapon>({sword, axe});

  const auto position = Vec3{1.0f, 2.0f, 3.0f};

  const auto inventory =
      std::vector<std::uint8_t>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  const auto orc = Monster{.pos = position,
                           .mana = 150,
                           .hp = 80,
                           .name = "MyMonster",
                           .inventory = inventory,
                           .color = Color::make<"Red">(),
                           .weapons = weapons,
                           .equipped = rfl::make_field<"weapon">(axe),
                           .path = rfl::default_value};

  write_and_read(orc);
}

