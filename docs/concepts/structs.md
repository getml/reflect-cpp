# Structs

Structs can be set up as follows:

```cpp
struct Person {
    std::string first_name;
    std::string last_name;
    std::vector<Person> children;
};
```

You can initilize your struct like this:

```cpp
const auto bart = Person{.first_name = "Bart",
                         .last_name = "Simpson",
                         .children = std::vector<Person>()};
```

JSON uses camel case, but C++ uses snake case, so you might want to rename your fields:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    std::vector<Person> children;
};
```


You can also initialize fields in the struct declaration itself:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name = "Simpson";
    std::vector<Person> children;
};
```

Because you now have explicitly set this field, you do not have to do this every time:

```cpp
const auto bart = Person{.first_name = "Bart"};

const auto lisa = Person{.first_name = "Lisa"};

const auto maggie = Person{.first_name = "Maggie"};

const auto homer = Person{.first_name = "Homer",
                          .children = std::vector<Person>({bart, lisa, maggie})};

```

You can write this to a JSON string:

```cpp
const auto json_string = rfl::json::write(homer);
```

```json
{"firstName":"Homer","lastName":"Simpson","children":[{"firstName":"Bart","lastName":"Simpson","children":[]},{"firstName":"Lisa","lastName":"Simpson","children":[]},{"firstName":"Maggie","lastName":"Simpson","children":[]}]}
```

And you can parse it back into a struct:

```cpp
const auto homer = rfl::json::read<Person>(json_string).value();
```

!!! note
    **Do not create custom constructors on the structs.**

    reflect-cpp needs to be able to create the structs like this:

    ```cpp
    Person{"Bart", "Simpson", ...};
    ```

    But if you create a custom constructor, then C++ will no longer allow this kind of constructions.

    If you want to create the struct from one of your classes (the most like reason, you want to create custom constructors in the first place),
    you might want to check out the section on [custom classes](https://github.com/getml/reflect-cpp/blob/main/docs/custom_classes.md) or [custom parsers](https://github.com/getml/reflect-cpp/blob/main/docs/custom_parser.md).


## Utility functions

### `rfl::as`

`rfl::as` allows you to cast structs as other structs. This is useful if the fields in `B` are a subset of the fields in `A`
or we need the fields in `A` and `B` to produce `C`.

This only works for the `rfl::Field`-syntax.

For example:

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

// Creates c by retrieving f1 and f2 from a and f4 from b. 
// Ignores f3.
const auto c = rfl::as<C>(a, b);
```

In this case, `f1`, `f2` and `f4` are now copied into the newly created `c` of type `C`.

Much like `rfl::replace`, `rfl::as` supports move semantics:

```cpp
auto a = A{.f1 = "Hello", .f2 = "World"};

auto b = B{.f3 = "Hello", .f4 = "World"};

const auto c = rfl::as<C>(std::move(a), std::move(b));
```

In this case, `f1`, `f2` and `f4` are now moved into the newly created `c` of type `C`. This is a zero-copy operation. `f3` is discarded.

`rfl::as` also supports `rfl::Flatten`. For instance:

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
    rfl::Flatten<A> a;
    rfl::Flatten<B> b;
    int f5;
};

const auto a = A{.f1 = "Hello", .f2 = "World"};

const auto b = B{.f3 = "Hello", .f4 = "World"};

const auto c =
    rfl::as<C>(a, b, rfl::make_field<"f5">(5));
```

In this particular instance, `rfl::as<C>(...)` is basically equivalent to this:

```cpp
const auto c = C{.a = a, .b = b, .f5 = rfl::make_field<"f5">(5)};
```

### `rfl::replace`

`rfl::replace` creates a deep copy of the original struct or moves the original struct, replacing one or several fields in the process.

This only works for the `rfl::Field`-syntax.

In some cases, we really only want to change one or a few fields, to get from one struct to another:

```cpp
const auto lisa = Person{
    .first_name = "Lisa",
    .last_name = "Simpson",
    .children = std::vector<Person>()
};

// Returns a deep copy of the original object,
// replacing first_name.
const auto maggie =
    rfl::replace(lisa, rfl::make_field<"first_name">(std::string("Maggie")));
```

`maggie` is now a deep copy of `lisa`, but with a new `first_name`.

However, in some cases, we do not want or are unable to create a deep copy of a struct.

For instance, suppose we had put the field `children` into a `rfl::Box`:

```cpp
struct Person {
    std::string first_name;
    std::string last_name;
    rfl::Box<std::vector<Person>> children;
};
```

`rfl::Box` cannot be copied, and if we naively try to apply `rfl::replace` to this,
this will not compile (disabling copies is very much the point of `rfl::Box`).

However, we can use `std::move`:

```cpp
auto lisa = Person{.first_name = "Lisa",
                   .last_name = "Simpson",
                   .children = rfl::make_box<std::vector<Person>>()};

const auto maggie = rfl::replace(
    std::move(lisa), rfl::make_field<"firstName">(std::string("Maggie")));
```

The fields from `lisa` have now been moved into `maggie`.

We can also remove several fields using `replace`:

```cpp
auto lisa = Person{.first_name = "Lisa",
                   .last_name = "Simpson",
                   .children = rfl::make_box<std::vector<Person>>()};

const auto milhouse = rfl::replace(
    std::move(lisa),
    rfl::make_field<"firstName">(std::string("Maggie")),
    rfl::make_field<"lastName">(std::string("Van Houten")));
```

If you have nested structs using `rfl::Flatten`, you can treat the fields inside `rfl::Flatten`
as if they were on the main level:

```cpp
struct Person {
    std::string first_name;
    rfl::Box<std::string> last_name;
    int age;
};

struct Employee {
    rfl::Flatten<Person> person;
    rfl::Box<std::string> employer;
    float salary;
};

auto employee = Employee{
    .person = Person{.first_name = "Homer",
                     .last_name = rfl::make_box<std::string>("Simpson"),
                     .age = 45},
    .employer = rfl::make_box<std::string>("Mr. Burns"),
    .salary = 60000.0};

auto employee2 =
    rfl::replace(std::move(employee), rfl::make_field<"salary">(70000.0),
                 rfl::make_field<"age">(46));
````

In this case `age` is part of `Person` and `salary` part ot `Employee`, but
because you have flattened them, you can treat them as if they were on
the same level.

You can also replace structs with other structs. Consider the following example:

```cpp
struct Person {
    std::string first_name;
    std::string last_name;
    int age;
};

struct Employee {
    rfl::Flatten<Person> person;
    std::string employer;
    float salary;
};

const auto employee = Employee{
    .person =
        Person{.first_name = "Homer", .last_name = "Simpson", .age = 45},
    .employer = std::string("Mr. Burns"),
    .salary = 60000.0};

const auto carl = Person{.first_name = "Carl", .last_name = "", .age = 45};

const auto employee2 = rfl::replace(employee, carl);
```

This code flattens the employee structs and then replaces all relevant fields with their counterparts contained in `carl`.

Finally, we get this JSON string:

```json
{"first_name":"Carl","last_name":"","age":45,"employer":"Mr. Burns","salary":60000.0}
```

Don't worry, this is fairly optimized and makes heavy use of forwarding. It does not make any copies than it absolutely has to.

### `rfl::to_view`

`rfl::to_view` allows you to create views on structs using which you can access an modify the fields of the structs just like a tuple.

Under-the-hood, a view is a `rfl::NamedTuple` containing pointers to the original fields.

For example:

```cpp
auto lisa = Person{.first_name = "Lisa", .last_name = "Simpson", .age = 8};

const auto view = rfl::to_view(lisa);

// Assigns the first field, thus modifying the struct 'lisa'.
*view.get<0>() = "Maggie";

// view.values() is a std::tuple containing
// pointers to the original fields.
*std::get<1>(view.values()) = "Simpson";

// You can also access fields by their name.
// The correctness will be ensured at compile time.
*view.get<"age">() = 0;

// You can also access fields like this.
*rfl::get<0>(view) = "Maggie";

// Or like this.
*rfl::get<"first_name">(view) = "Maggie";
```
