# Custom classes

Reflection implies that all your fields are public. But in object-oriented programming, you often don't want
that. If your class is more than a trivial, behaviorless struct, you often want to make your fields private.

If you want your class to be supported by reflect-cpp, it needs to have the following:

1) It needs to publicly define a type called `ReflectionType` using `using` or `typedef`.
2) It needs to have a constructor that accepts your `ReflectionType` as an argument.
3) It needs to contain a method called `reflection` that returns said `ReflectionType` (or a reference thereto).

If you class fulfills these three conditions, then it is fully supported by all serialization and deserialization
routines in reflect-cpp.

If you absolutely do not want to make any changes to your original class, you can implement a [custom parser](https://github.com/getml/reflect-cpp/blob/main/docs/custom_parser.md).

## Example 1: Using an Impl struct

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

## Example 2: Matching variables, the safe way

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

## Example 3: Matching variables, the unsafe way

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
