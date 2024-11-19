# Flexbuffers

For flexbuffers support, you must also include the header `<rfl/flexbuf.hpp>` and link to the flatbuffers library (https://github.com/google/flatbuffers).

Flexbuffers is part of the flatbuffers library, which is a binary format developed by Google.

reflect-cpp can be used on top of flexbuffers. To see how this is advantageous, consider the following example:

## Simple example

```cpp
#include <rfl.hpp>
#include <rfl/flexbuf.hpp>

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

const auto res = rfl::flexbuf::read<Monster>(bytes);
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


But what it more, unlike "normal" flatbuffers, flexbuffers also supports field names. Field names make it a lot easier to maintain backwards compatability.

## Reading and writing

Suppose you have a struct like this:

```cpp
struct Person {
    std::string first_name;
    std::string last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;
    std::vector<Person> children;
};
```

A `person` can be turned into a bytes vector like this:

```cpp
const auto person = Person{...};
const auto bytes = rfl::flexbuf::write(person);
```

You can parse bytes like this:

```cpp
const rfl::Result<Person> result = rfl::flexbuf::read<Person>(bytes);
```

## Loading and saving

You can also load and save to disc using a very similar syntax:

```cpp
const rfl::Result<Person> result = rfl::flexbuf::load<Person>("/path/to/file.fb");

const auto person = Person{...};
rfl::flexbuf::save("/path/to/file.fb", person);
```

## Reading from and writing into streams

You can also read from and write into any `std::istream` and `std::ostream` respectively.

```cpp
const rfl::Result<Person> result = rfl::flexbuf::read<Person>(my_istream);

const auto person = Person{...};
rfl::flexbuf::write(person, my_ostream);
```

Note that `std::cout` is also an ostream, so this works as well:

```cpp
rfl::flexbuf::write(person, std::cout) << std::endl;
```

(Since flexbuffers is a binary format, the readability of this will be limited, but it might be useful for debugging).

## Custom constructors

One of the great things about C++ is that it gives you control over
when and how you code is compiled.

For large and complex systems of structs, it is often a good idea to split up
your code into smaller compilation units. You can do so using custom constructors.

For the flexbuffers format, these must be a static function on your struct or class called
`from_flexbuf` that take a `rfl::flexbuf::Reader::InputVarType` as input and return
the class or the class wrapped in `rfl::Result`.

In your header file you can write something like this:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    rfl::Timestamp<"%Y-%m-%d"> birthday;

    using InputVarType = typename rfl::flexbuf::Reader::InputVarType;
    static rfl::Result<Person> from_flexbuf(const InputVarType& _obj);
};
```

And in your source file, you implement `from_flexbuf` as follows:

```cpp
rfl::Result<Person> Person::from_flexbuf(const InputVarType& _obj) {
    const auto from_nt = [](auto&& _nt) {
        return rfl::from_named_tuple<Person>(std::move(_nt));
    };
    return rfl::flexbuf::read<rfl::named_tuple_t<Person>>(_obj)
        .transform(from_nt);
}
```

This will force the compiler to only compile the flexbuffers parsing when the
source file is compiled.
