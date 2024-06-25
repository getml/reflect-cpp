
Reflection implies that all your fields are public. But in object-oriented programming, you often don't want
that. If your class is more than a trivial, behaviorless struct, you often want to make your fields private.

If you want your class to be supported by reflect-cpp, it needs to have the following:

1. It needs to publicly define a type called `ReflectionType` using `using` or `typedef`.
2. It needs to have a constructor that accepts your `ReflectionType` as an argument.
3. It needs to contain a method called `reflection` that returns said `ReflectionType` (or a reference thereto).

If you class fulfills these three conditions, then it is fully supported by all serialization and deserialization
routines in reflect-cpp.

If you absolutely do not want to make any changes to your original class, you can implement a [custom parser](https://github.com/getml/reflect-cpp/blob/main/docs/custom_parser.md).

## Using an Impl struct

```cpp
struct PersonImpl {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    int age;
};

class Person {
    public:
      // 1) Publicly define `ReflectionType`
      using ReflectionType = PersonImpl;

      // 2) Constructor that accepts your `ReflectionType`
      Person(const PersonImpl& _impl): impl(_impl) {}

      ~Person() = default;

      // 3) Method called `reflection` that returns `ReflectionType`
      const ReflectionType& reflection() const { return impl; }

      // ...add some more methods here...

    private:
        PersonImpl impl;
};
```

## Matching variables, the safe way

`rfl::Field` is designed in a way that you have to explicitly initialize
every the field (using `rfl::default_value`, if necessary), otherwise
you will get a compile-time error. 

A frequent error that happens during serialization/deserialization is that programmers
add a field to their class (`Person` in this example), but forget to update
their serialization routine.

The example as shown below will protect you from any such errors, as all 
fields will have to be explicitly initialized, otherwise you will get a 
compile-time error. If you add a new field to `Person` you will have to
add it to `PersonImpl` as well and then explicitly initialize it in the 
constructor.

Don't worry `operator()` in `rfl::Field` is inlined. There won't be any 
runtime overhead.

```cpp
struct PersonImpl {
    rfl::Field<"firstName", std::string> first_name;
    rfl::Field<"lastName", std::string> last_name;
    rfl::Field<"age", int> age;
};

class Person {
    public:
      // 1) Publicly define `ReflectionType`
      using ReflectionType = PersonImpl;

      // 2) Constructor that accepts your `ReflectionType`
      // This as the additional benefit that not only the types,
      // but also the names of the fields will be checked at compile time.
      Person(const PersonImpl& _impl): first_name(_impl.first_name),
          last_name(_impl.last_name), age(_impl.age) {}

      ~Person() = default;

      // 3) Method called `reflection` that returns `ReflectionType`
      ReflectionType reflection() const {
          return PersonImpl{
            .first_name = first_name,
            .last_name = last_name,
            .age = age};
      }

      // ...add some more methods here...

    private:
      rfl::Field<"firstName", std::string> first_name;
      rfl::Field<"lastName", std::string> last_name;
      rfl::Field<"age", int> age;
};
```

## Matching variables, the unsafe way

If, for any reason, you absolutely cannot change the fields
of your class, you have to make sure that all classes are properly
initialized or face runtime errors.

```cpp
struct PersonImpl {
    // ... same as in Example 1 or 2
};

class Person {
    // 1) Publicly define `ReflectionType`
    using ReflectionType = PersonImpl;

    // 2) Constructor that accepts your `ReflectionType`
    Person(const PersonImpl& _impl): first_name(_impl.first_name()),
        last_name(_impl.last_name()), age(_impl.age()) {}

    // ... same as in Example 2
    
    private:
      std::string first_name;
      std::string last_name;
      int age;
};
```

## Custom parsers

If you absolutely do not want to make any changes to your original classes whatsoever,
you can implement a custom parser using `rfl::parsing::CustomParser`.

In order to do so, you must do the following:

You must create a helper struct that *can* be parsed. The helper struct must fulfill the following
conditions:

1. It must contain a static method called `from_class` that takes your original class as an input and returns the helper struct. This method must not throw an exception.

2. (Optional) It must contain a method called `to_class` that transforms the helper struct into your original class. This method may throw an exception, if you want to. If you can directly construct your custom class from the field values in the order they were declared in the helper struct, you do not have to write a `to_class` method.

You can then implement a custom parser for your class like this:

```cpp
namespace rfl::parsing {

template <class ReaderType, class WriterType, class ProcessorsType>
struct Parser<ReaderType, WriterType, YourOriginalClass, ProcessorsType>
    : public CustomParser<ReaderType, WriterType, ProcessorsType, YourOriginalClass,
                          YourHelperStruct> {};

}  // namespace rfl::parsing
```

**Example**

Suppose your original class looks like this:

```cpp
struct Person {
    Person(const std::string& _first_name, const std::string& _last_name,
           const int _age)
        : first_name_(_first_name), last_name_(_last_name), age_(_age) {}

    const auto& first_name() const { return first_name_; }

    const auto& last_name() const { return last_name_; }

    auto age() const { return age_; }

   private:
    std::string first_name_;
    std::string last_name_;
    int age_;
};
```

You can then write a helper struct: 

```cpp
struct PersonImpl {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    int age;

    // 1) Static method that takes your original class as an input and
    //    returns the helper struct.
    //    MUST NOT THROW AN EXCEPTION!
    static PersonImpl from_class(const Person& _p) noexcept {
        return PersonImpl{.first_name = _p.first_name(),
                          .last_name = _p.last_name(),
                          .age = _p.age()};
    }

    // 2) Const method called `to_class` that transforms the helper struct
    //    into your original class.
    //    In this case, the `to_class` method is actually optional, because
    //    you can directly create Person from the field values.
    Person to_class() const { return Person(first_name(), last_name(), age); }
};
```

You then implement the custom parser:

```cpp
namespace rfl::parsing {

template <class ReaderType, class WriterType, class ProcessorsType>
struct Parser<ReaderType, WriterType, Person>
    : public CustomParser<ReaderType, WriterType, ProcessorsType, Person, PersonImpl> {};

}  // namespace rfl::parsing
```

Now your custom class is fully supported by reflect-cpp. So for instance, you could parse it
inside a vector:

```cpp
const auto people = rfl::json::read<std::vector<Person>>(json_str).value();
```

As we have noted, in this particular example, the `Person` class can be constructed from the field values in
`PersonImpl` in the exact same order they were declared in `PersonImpl`. So we can drop the `.to_class` method:

```cpp
struct PersonImpl {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    int age;

    static PersonImpl from_class(const Person& _p) noexcept {
        return PersonImpl{.first_name = _p.first_name(),
                          .last_name = _p.last_name(),
                          .age = _p.age()};
    }
};
```
