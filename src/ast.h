#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <optional>
#include <variant>

std::vector<std::string> existing_functions;
std::vector<std::string> existing_types;
std::vector<std::string> existing_variables;

class Node {
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

    std::string toString() {
        std::string result = type;
        if (leaf.has_value()) {
            result += " " + leaf.value();
        }
        if (!children.empty()) {
            result += " (";
            for (const auto& child : children) {
                result += child->toString() + " ";
            }
            result = result.substr(0, result.size() - 1) + ")";
        }
        return result;
    }
};