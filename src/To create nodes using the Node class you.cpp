To create nodes using the Node class you've defined, you would use its constructors in different ways depending on what you want each node to contain. Here are the commands for each type of node you've mentioned:

    Node with just a type:

    cpp

Node nodeWithTypeOnly("TypeOnly");

Node with a type and children:

cpp

std::vector<std::unique_ptr<Node>> children;
children.push_back(std::make_unique<Node>("Child1"));
children.push_back(std::make_unique<Node>("Child2"));
Node nodeWithTypeAndChildren("TypeWithChildren", std::move(children));

Node with a type and a leaf:

cpp

Node nodeWithTypeAndLeaf("TypeWithLeaf", std::optional<std::string>("Leaf"));

Node with a type, a leaf, and children:

cpp

    std::vector<std::unique_ptr<Node>> childrenWithLeaf;
    childrenWithLeaf.push_back(std::make_unique<Node>("Child1"));
    childrenWithLeaf.push_back(std::make_unique<Node>("Child2"));
    Node nodeWithTypeLeafAndChildren("TypeWithLeafAndChildren",
                                         std::move(childrenWithLeaf), 
                                         std::optional<std::string>("Leaf"));
