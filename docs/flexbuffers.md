# Flexbuffers

For flexbuffers support, you must also include the header `<rfl/flexbuf.hpp>` and link to the flatbuffers library (https://github.com/google/flatbuffers).

Flexbuffers is part of the flatbuffers library, which is a binary format developed by Google.

reflect-cpp can be used on top of flexbuffers. To see how this is advantageous, consider the following example:

## Simple example

```cpp
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
  std::vector<std::uint8_t> inventory;
  Color color = Color::make<"Blue">();
  std::vector<Weapon> weapons;
  Equipment equipped;
  std::vector<Vec3> path;
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
                         .equipped = rfl::make_field<"weapon">(axe)};

const auto bytes = rfl::flexbuf::write(orc);

const auto res = rfl::flexbuf::read<Monster>(orc);
```

## For comparison: Standard flatbuffers

Let's talk about what normal flatbuffers would make you do to set up this example.

First of all, you would have to set up your `Monster.fbs`:

```
namespace MyGame.Sample;

enum Color:byte { Red = 0, Green, Blue = 2 }

union Equipment { Weapon } // Optionally add more tables.

struct Vec3 {
  x:float;
  y:float;
  z:float;
}

table Monster {
  pos:Vec3;
  mana:short = 150;
  hp:short = 100;
  name:string;
  friendly:bool = false (deprecated);
  inventory:[ubyte];
  color:Color = Blue;
  weapons:[Weapon];
  equipped:Equipment;
  path:[Vec3];
}

table Weapon {
  name:string;
  damage:short;
}

root_type Monster;
```

Then you would have to generate the C++ code using the flatbuffers compiler.

Finally, here is the code that you would then write afterwards:

```cpp
// Build up a serialized buffer algorithmically:
flatbuffers::FlatBufferBuilder builder;

// First, lets serialize some weapons for the Monster: A 'sword' and an 'axe'.
auto weapon_one_name = builder.CreateString("Sword");
short weapon_one_damage = 3;

auto weapon_two_name = builder.CreateString("Axe");
short weapon_two_damage = 5;

// Use the `CreateWeapon` shortcut to create Weapons with all fields set.
auto sword = CreateWeapon(builder, weapon_one_name, weapon_one_damage);
auto axe = CreateWeapon(builder, weapon_two_name, weapon_two_damage);

// Create a FlatBuffer's `vector` from the `std::vector`.
std::vector<flatbuffers::Offset<Weapon>> weapons_vector;
weapons_vector.push_back(sword);
weapons_vector.push_back(axe);
auto weapons = builder.CreateVector(weapons_vector);

// Second, serialize the rest of the objects needed by the Monster.
auto position = Vec3(1.0f, 2.0f, 3.0f);

auto name = builder.CreateString("MyMonster");

unsigned char inv_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
auto inventory = builder.CreateVector(inv_data, 10);

// Shortcut for creating monster with all fields set:
auto orc = CreateMonster(builder, &position, 150, 80, name, inventory,
                         Color_Red, weapons, Equipment_Weapon, axe.Union());

builder.Finish(orc);  // Serialize the root of the object.
```

I think it should be fairly obvious that using reflect-cpp on top drastically reduces the amount of boilerplate code.

## Reading and writing

Suppose you have a struct like this:

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;
    rfl::Field<"children", std::vector<Person>> children;
};
```

You can parse JSON strings like this:

```cpp
const rfl::Result<Person> result = rfl::json::read<Person>(json_string);
```

A person can be turned into a JSON string like this:

```cpp
const auto person = Person{...};
const std::string json_string = rfl::json::write(person);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::json::load<Person>("/path/to/file.json");

const auto person = Person{...};
rfl::json::save("/path/to/file.json", person);
```

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the JSON format, these must be a static function on your struct or class called
`from_json_obj` that take a `rfl::json::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"birthday", rfl::Timestamp<"%Y-%m-%d">> birthday;

    using JSONVar = typename rfl::json::Reader::InputVarType;
    static rfl::Result<Person> from_json_obj(const JSONVar& _obj);
};
```

And in your source file, you implement `from_json_obj` as follows:

```cpp
rfl::Result<Person> Person::from_json_obj(const JSONVar& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::json::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the JSON parsing when the
source file is compiled.
