#pragma once

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

std::vector<std::string> existing_functions;
std::vector<std::string> existing_types;
std::vector<std::string> existing_variables;

class Node
{
public:
    std::string type;
    std::vector<std::unique_ptr<Node>> children;
    std::optional<std::string> leaf;

    Node(std::string type, std::vector<std::unique_ptr<Node>> children, std::optional<std::string> leaf)
        : type(std::move(type)), children(std::move(children)), leaf(std::move(leaf)) {}

    Node(std::string type, std::vector<std::unique_ptr<Node>> children)
        : type(std::move(type)), children(std::move(children)) {}

    Node(std::string type, std::optional<std::string> leaf)
        : type(std::move(type)), leaf(std::move(leaf)) {}

    Node(std::string type) : type(std::move(type)) {}

    Node() {}

    std::string toString() const
    {
        std::stringstream ss;
        ss << type;
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
