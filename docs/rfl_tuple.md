# `rfl::Tuple` 

The problem with `std::tuple` is that it is recursively implemented. That is fine when you
have a small number of elements in your tuple, but as you increase the size of the tuple,
you will pay a price in terms of compile time.

That is why reflect-cpp contains `rfl::Tuple`, which in many ways behaves 
just like `std::tuple`, but it compiles considerably faster.

The only downside of `rfl::Tuple` is that it cannot be used inside `constexpr` or `consteval`
functions.

You can use the functions `rfl::get`, `rfl::make_tuple`, `rfl::tuple_element_t`, 
`rfl::tuple_size_v`, `rfl::tuple_cat` or `rfl::apply` to create or access the 
tuple and they work the same way as their equivalents in the standard library.
In fact, `std::get` will also work on `rfl::Tuple`.
