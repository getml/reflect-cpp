# `rfl::Box` and `rfl::Ref` 

In previous sections, we have defined the `Person` class recursively:

```cpp
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    std::vector<Person> children;
};
```

This works, because `std::vector` contains a pointer under-the-hood. But what wouldn't work is something like this:

```cpp
// WILL NOT COMPILE
struct Person {
    rfl::Rename<"firstName", std::string> first_name;
    rfl::Rename<"lastName", std::string> last_name;
    Person child;
};
```

This is because the compiler cannot figure out the intended size of the struct. But recursively defined structures
are important. For instance, if you deal with machine learning, you might be familiar with a decision tree.

A decision tree consists of a `Leaf` containing the prediction and a `Node` which splits the decision tree into
two subtrees.

A naive implementation might look like this:

```cpp
// WILL NOT COMPILE
struct DecisionTree {
    struct Leaf {
        using Tag = rfl::Literal<"Leaf">;
        double prediction;
    };

    struct Node {
        using Tag = rfl::Literal<"Node">;
        rfl::Rename<"criticalValue", double> critical_value;
        DecisionTree lesser;
        DecisionTree greater;
    };

    using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

    rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};
```

Again, this will not compile, because the compiler cannot figure out the intended size of the struct.

A possible solution might be to use `std::unique_ptr`:

```cpp
// Will compile, but not an ideal design.
struct DecisionTree {
    struct Leaf {
        using Tag = rfl::Literal<"Leaf">;
        double prediction;
    };

    struct Node {
        using Tag = rfl::Literal<"Node">;
        rfl::Rename<"criticalValue", double> critical_value;
        std::unique_ptr<DecisionTree> lesser;
        std::unique_ptr<DecisionTree> greater;
    };

    using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

    rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};
```

This will compile, but the design is less than ideal. We know for a fact that a `Node` must have
exactly two subtrees. But this is not reflected in the type system. In this encoding, the fields 
"lesser" and "greater" are marked optional and you will have to check at runtime that they are indeed set.

But this violates the principles of reflection. Reflection is all about validating as much of our assumptions
upfront as we possibly can. For a great theoretical discussion of this topic, check out 
[Parse, don't validate](https://lexi-lambda.github.io/blog/2019/11/05/parse-don-t-validate/)
by Alexis King.

So how would we encode our assumptions that the fields "lesser" and "greater" must exist in the type system and
still have code that compiles? By using `rfl::Box` instead of `std::unique_ptr`:

```cpp
struct DecisionTree {
    struct Leaf {
        using Tag = rfl::Literal<"Leaf">;
        double prediction;
    };

    struct Node {
        using Tag = rfl::Literal<"Node">;
        rfl::Rename<"criticalValue", double> critical_value;
        rfl::Box<DecisionTree> lesser;
        rfl::Box<DecisionTree> greater;
    };

    using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

    rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};
```

`rfl::Box` is a thin wrapper around `std::unique_ptr`, but it is guaranteed to **never be null** (unless you do something egregious such as trying to access it after calling `std::move`). It is a `std::unique_ptr` without the `nullptr`.

If you want to learn more about the evils of null references, check out the 
[Null References: The Billion Dollar Mistake](https://www.infoq.com/presentations/Null-References-The-Billion-Dollar-Mistake-Tony-Hoare/)
by Tony Hoare, who invented the concept in the first place.

You **must** initialize `rfl::Box` the moment you create it and it cannot be dereferenced until it is destroyed.

`rfl::Box` can be initialized using `rfl::make_box<...>(...)`, just like `std::make_unique<...>(...)`:

```cpp
auto leaf1 = DecisionTree::Leaf{.value = 3.0};

auto leaf2 = DecisionTree::Leaf{.value = 5.0};

auto node =
    DecisionTree::Node{.critical_value = 10.0,
                       .lesser = rfl::make_box<DecisionTree>(leaf1),
                       .greater = rfl::make_box<DecisionTree>(leaf2)};

const DecisionTree tree{.leaf_or_node = std::move(node)};

const auto json_string = rfl::json::write(tree);
```

This will result in the following JSON string:

```json
{"leafOrNode":{"type":"Node","criticalValue":10.0,"lesser":{"leafOrNode":{"type":"Leaf","value":3.0}},"greater":{"leafOrNode":{"type":"Leaf","value":5.0}}}}
```

You can also initialize `rfl::Box<T>` from a `std::unique_ptr<T>`:

```cpp
auto ptr = std::make_unique<std::string>("Hello World!");
const rfl::Result<rfl::Box<std::string>> box = rfl::make_box<std::string>(std::move(ptr));
```

Note that `box` is wrapped in a `Result`. That is, because we cannot guarantee at compile time 
that `ptr` is not `nullptr`, therefore we need to account for that.

If you want to use reference-counted pointers, instead of unique pointers, you can use `rfl::Ref`. 
`rfl::Ref` is the same concept as `rfl::Box`, but using `std::shared_ptr` under-the-hood.

```cpp
struct DecisionTree {
    struct Leaf {
        using Tag = rfl::Literal<"Leaf">;
        double value;
    };

    struct Node {
        using Tag = rfl::Literal<"Node">;
        rfl::Rename<"criticalValue", double> critical_value;
        rfl::Ref<DecisionTree> lesser;
        rfl::Ref<DecisionTree> greater;
    };

    using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

    rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};

const auto leaf1 = DecisionTree::Leaf{.value = 3.0};

const auto leaf2 = DecisionTree::Leaf{.value = 5.0};

auto node =
    DecisionTree::Node{.critical_value = 10.0,
                       .lesser = rfl::make_ref<DecisionTree>(leaf1),
                       .greater = rfl::make_ref<DecisionTree>(leaf2)};

const DecisionTree tree{.leaf_or_node = std::move(node)};

const auto json_string = rfl::json::write(tree);
```

The resulting JSON string is identical:

```json
{"leafOrNode":{"type":"Node","criticalValue":10.0,"lesser":{"leafOrNode":{"type":"Leaf","value":3.0}},"greater":{"leafOrNode":{"type":"Leaf","value":5.0}}}}
```



