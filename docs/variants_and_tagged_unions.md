# Variants and Tagged Unions

## Variants

Sometimes you know that the JSON object can be one of several alternatives. For example,
you might have several shapes like `Circle`, `Rectangle` or `Square`. For these kind of 
cases, the standard library contains `std::variant`:

```cpp
struct Circle {
    rfl::Field<"radius", double> radius;
};

struct Rectangle {
    rfl::Field<"height", double> height;
    rfl::Field<"width", double> width;
};

struct Square {
    rfl::Field<"width", double> width;
};

using Shapes = std::variant<Circle, Rectangle, Square>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

This code will compile just fine and work as intended. However, there are
several problems with this:

1) It is in inefficient: The parser has to read the fields for all of the different alternatives until it can't find a required field in the JSON object. It will then move on to the next alternative.
2) It leads to confusing error messages: If none of the alternatives can be matched, you will get an error message telling you why each of the alternatives couldn't be matched. Such error messages are very long-winding and hard to read.
3) It is dangerous. Imagine we had written `std::variant<Circle, Square, Rectangle>` instead of `std::variant<Circle, Rectangle, Square>`. This would mean that `Rectangle` could never be matched, because the fields in `Square` are a subset of `Rectangle`. This leads to very confusing bugs.

## Tagged Unions

That is why there is an alternative: `rfl::TaggedUnion`.

TaggedUnions require some kind of identifying field on your struct, which must be expressed as an `rfl::Literal`. It will then try to take that field from the JSON object, match it to the correct alternative and then only parse the correct alternative.

We will now rewrite the example from above using a Tagged Union:

```cpp
struct Circle {
    // All alternatives must contain a field named "shape". The type
    // of the field must be an rfl::Literal. Since that Literal usually
    // only contains one string (even though it doesn't have to), you can
    // assign it to its default value.
    rfl::Field<"shape", rfl::Literal<"Circle">> shape = rfl::default_value;
    rfl::Field<"radius", double> radius;
};

struct Rectangle {
    rfl::Field<"shape", rfl::Literal<"Rectangle">> shape =
        rfl::default_value;
    rfl::Field<"height", double> height;
    rfl::Field<"width", double> width;
};

struct Square {
    rfl::Field<"shape", rfl::Literal<"Square">> shape = rfl::default_value;
    rfl::Field<"width", double> width;
};

// Now you tell rfl::TaggedUnion that you want it to look for the field "shape".
using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes r = Rectangle{.height = 10, .width = 5};

const auto json_string = rfl::json::write(r);

const auto r2 = rfl::json::read<Shapes>(json_string);
```

This solves all of the three problems we have listed above.

## Resolving tagged unions

`rfl::TaggedUnion` is just an `std::variant` under-the-hood. The original variant can be
retrieved using `.variant()`. You can then resolve it using the [visitor pattern](https://en.cppreference.com/w/cpp/utility/variant/visit):

```cpp
using Shapes = rfl::TaggedUnion<"shape", Circle, Square, Rectangle>;

const Shapes my_shape = Rectangle{.height = 10, .width = 5};

const auto handle_shapes = [](const auto& s) {
  using Type = std::decay_t(decltype(s));
  if constexpr (std::is_same<Type, Circle>()) {
     std::cout << is circle, radius: << s.radius() << std::endl;
  } else if constexpr (std::is_same<Type, Rectangle>()) {
     std::cout << is rectangle, width: << s.width() << ", height: " << s.height() << std::endl;
  } else if constexpr (std::is_same<Type, Square>()) {
     std::cout << is square, width: << s.width() << std::endl;
  } else {
    // reflect-cpp also provides this very useful helper that ensures
    // at compile-time that you didn't forget anything.
    static_assert(rfl::always_false_v<Type>, "Not all cases were covered.");
  }
};

std::visit(handle_shapes, my_shape.variant());
```
