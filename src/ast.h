#ifndef AST_H
#define AST_H

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include <variant>

class Node
{
public:
    // Constructors
    Node(std::string name, std::vector<std::unique_ptr<Node>> children, std::optional<std::string> leaf)
        : name(std::move(name)), children(std::move(children)), leaf(std::move(leaf)) {}

    Node(std::string name, std::vector<std::unique_ptr<Node>> children)
        : name(std::move(name)), children(std::move(children)) {}

    Node(std::string name, std::optional<std::string> leaf)
        : name(std::move(name)), leaf(std::move(leaf)) {}

    Node(std::string name) : name(std::move(name)) {}

    Node() {}

    // Member variables
    std::string name;                            // Name or type of the node
    std::optional<std::string> leaf;             // Optional string leaf
    std::vector<std::unique_ptr<Node>> children; // Vector of child nodes

    std::string toString() const
    {
        std::stringstream ss;
        ss << name;
        if (leaf.has_value())
        {
            ss << " " << leaf.value();
        }
        if (!children.empty())
        {
            ss << " (";
            for (const auto &child : children)
            {
                ss << child->toString() << " ";
            }
            ss.seekp(-1, std::ios_base::end);
            ss << ")";
        }
        return ss.str();
    }
};

#endif // AST_H