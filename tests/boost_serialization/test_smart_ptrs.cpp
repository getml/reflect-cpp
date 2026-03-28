#include <memory>
#include <rfl.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_smart_ptrs {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::unique_ptr<std::vector<Person>> children;
};

TEST(boost_serialization, test_unique_ptr) {
  auto children = std::make_unique<std::vector<Person>>();
  children->emplace_back(Person{.first_name = "Bart"});
  children->emplace_back(Person{.first_name = "Lisa"});
  const auto homer =
      Person{.first_name = "Homer", .children = std::move(children)};
  write_and_read(homer);
}

struct PersonShared {
  std::string first_name;
  std::shared_ptr<std::vector<PersonShared>> children;
};

TEST(boost_serialization, test_shared_ptr) {
  auto children = std::make_shared<std::vector<PersonShared>>();
  children->emplace_back(PersonShared{.first_name = "Bart"});
  const auto homer =
      PersonShared{.first_name = "Homer", .children = std::move(children)};
  write_and_read_with_json(homer);
}

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

TEST(boost_serialization, test_box) {
  auto node = DecisionTree::Node{
      .critical_value = 10.0,
      .lesser = rfl::make_box<DecisionTree>(
          DecisionTree{DecisionTree::Leaf{.value = 3.0}}),
      .greater = rfl::make_box<DecisionTree>(
          DecisionTree{DecisionTree::Leaf{.value = 5.0}})};
  const DecisionTree tree{.leaf_or_node = std::move(node)};
  write_and_read_with_json(tree);
}

}  // namespace test_smart_ptrs
