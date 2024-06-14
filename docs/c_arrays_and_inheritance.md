# C Arrays and inheritance 

reflect-cpp supports fixed-size C arrays and some forms of inheritance, but only as an opt-in.

We have made this design decision, because checking for C arrays and inheritance 
requires a disproportionate amount of compile time and we believe it is
more important to reduce compile time than to support things out-of-the-box
that most people won't need anyway. Since there are good alternatives for 
both of these problems, it is recommended to avoid using C arrays or 
inheritance altogether.

Note that C arrays are not the same thing as `std::array`. `std::array` is always
supported and is the recommended alternative.

If you want support for these, you will have to pass the flag `-D REFLECT_CPP_C_ARRAYS_OR_INHERITANCE`
during compilation.

## C arrays

Suppose you have a struct like this:

```cpp
struct Person{
    std::string first_name;  
    std::string last_name; 
    int[5] post_code;
};
```

In this example, `post_code` is a C array. The best way to handle this
is to simply replace the C array with `std::array`, like this:

```cpp
struct Person{
    std::string first_name;  
    std::string last_name; 
    std::array<int, 5> post_code;
};
```

However, in some cases, using C arrays is unavoidable. For instance `bson_oid_t` contains
a C array under-the-hood, so if you want to parse a `bson_oid_t` you will have
no choice but to pass the flag mentioned above.

## Inheritance

reflect-cpp supports some form of inheritance. Because we use structured bindings to
retrieve the fields of a struct, there are limitations on what we can do when it comes
to inheritance:

*Every non-static data member of E must be a direct member of E or the same base class of E.* 
([https://en.cppreference.com/w/cpp/language/structured_binding](https://en.cppreference.com/w/cpp/language/structured_binding))

In other words, all of the fields must be inside the same struct.

So, this is fine:

```cpp
struct Base {
 int x;
};

struct Derived : Base {};
```

This is fine as well:

```cpp
struct Base {};
struct Derived : Base {
  int x;
};
```

But this is not fine, because the fields
are spread out over more than one struct:

```cpp
struct Base {
   int x;
};

struct Derived : Base {
  int y;
};
```

The recommended alternative is to simply use `rfl::Flatten`, which
has no such limitation:

```cpp
struct Base {
   int x;
};

struct Derived {
  rfl::Flatten<Base> base; 
  int y;
};
```

Please refer to the section on `rfl::Flatten` in this documentation for
more information.

If for any reason you want to use inheritance patterns like the ones
described above, you will make have to make sure that all of the fields
are inside the same struct and also you will also have to pass the 
compile time flag mentioned above.
