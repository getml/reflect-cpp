#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <source_location>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_box {

struct DecisionTree {
  struct Leaf {
    using Tag = rfl::Literal<"Leaf">;
    double value;
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

TEST(json, test_box) {
  auto leaf1 = DecisionTree::Leaf{.value = 3.0};

  auto leaf2 = DecisionTree::Leaf{.value = 5.0};

  auto node = DecisionTree::Node{
      .critical_value = 10.0,
      .lesser = rfl::make_box<DecisionTree>(DecisionTree{leaf1}),
      .greater = rfl::make_box<DecisionTree>(DecisionTree{leaf2})};

  const DecisionTree tree{.leaf_or_node = std::move(node)};

  write_and_read(
      tree,
      R"({"leafOrNode":{"type":"Node","criticalValue":10.0,"lesser":{"leafOrNode":{"type":"Leaf","value":3.0}},"greater":{"leafOrNode":{"type":"Leaf","value":5.0}}}})");
}
}  // namespace test_box
