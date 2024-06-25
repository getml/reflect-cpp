## Reflective programming

Beyond serialization and deserialization, reflect-cpp also supports reflective programming in general.

For instance:

```cpp
struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::string town = "Springfield";
  unsigned int age;
  std::vector<Person> children;
};

for (const auto& f : rfl::fields<Person>()) {
  std::cout << "name: " << f.name() << ", type: " << f.type() << std::endl;
}
```

You can also create a view and then access these fields using `std::get` or `rfl::get`, or iterate over the fields at compile-time:

```cpp
auto lisa = Person{.first_name = "Lisa", .last_name = "Simpson", .age = 8};

const auto view = rfl::to_view(lisa);

// view.values() is a std::tuple containing
// pointers to the original fields.
// This will modify the struct `lisa`:
*std::get<0>(view.values()) = "Maggie";

// All of this is supported as well:
*view.get<1>() = "Simpson";
*view.get<"age">() = 0;
*rfl::get<0>(view) = "Maggie";
*rfl::get<"first_name">(view) = "Maggie";

view.apply([](const auto& f) {
  // f is a an rfl::Field pointing to the original field.
  std::cout << f.name() << ": " << rfl::json::write(*f.value()) << std::endl;
});
```

It also possible to replace fields:

```cpp
struct Person {
  std::string first_name;
  std::string last_name;
  std::vector<Person> children;
};

const auto lisa = Person{.first_name = "Lisa", .last_name = "Simpson"};

// Returns a deep copy of "lisa" with the first_name replaced.
const auto maggie = rfl::replace(
    lisa, rfl::make_field<"first_name">(std::string("Maggie")));
```

Or you can create structs from other structs:

```cpp
struct A {
  std::string f1;
  std::string f2;
};

struct B {
  std::string f3;
  std::string f4;
};

struct C {
  std::string f1;
  std::string f2;
  std::string f4;
};

const auto a = A{.f1 = "Hello", .f2 = "World"};

const auto b = B{.f3 = "Hello", .f4 = "World"};

// f1 and f2 are taken from a, f4 is taken from b, f3 is ignored.
const auto c = rfl::as<C>(a, b);
```

You can also replace fields in structs using fields from other structs:

```cpp
const auto a = A{.f1 = "Hello", .f2 = "World"};

const auto c = C{.f1 = "C++", .f2 = "is", .f4 = "great"};

// The fields f1 and f2 are replaced with the fields f1 and f2 in a.
const auto c2 = rfl::replace(c, a);
```

