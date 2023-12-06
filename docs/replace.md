# `rfl::replace`

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

