#pragma once
#include <optional>
#include <variant>
#include <iostream>
#include <string>
#include <vector>
#include "ansi.h"
#include "token.h"

struct NodeExprIntLit {
    Token int_lit;
};

struct NodeExprIdent {
    Token ident;
};

struct NodeExpr {
    std::variant<NodeExprIntLit, NodeExprIdent> var;
};

struct NodeStmtExit {
    NodeExpr expr;
};

struct NodeStmtInt {
    Token ident;
    NodeExpr expr;
};

struct NodeStmt {
    std::variant<NodeStmtExit, NodeStmtInt> var;
};

struct NodeProg {
    std::vector<NodeStmt> stmts;
};

class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens)
        : m_tokens(std::move(tokens))
    {}

    std::optional<NodeExpr> parse_expr()
    {
        if (peak().has_value() && peak().value().type == TokenType::int_lit)
        {
            return NodeExpr{.var = NodeExprIntLit{.int_lit = consume()}};
        } else if (peak().has_value() && peak().value().type == TokenType::ident) {
            return NodeExpr{.var = NodeExprIdent{.ident = consume()}};
        } else {
            return {};
        }
    }

    std::optional<NodeStmt> parse_stmt()
    {
        if (peak().has_value() && peak().value().type == TokenType::_exit) {
            consume();
            if (peak().has_value() && peak().value().type == TokenType::open_bracket) {
                consume();
            } else {
                std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Expected '('" << std::endl;
                std::cerr << "compilation terminated." << std::endl;
                exit(EXIT_FAILURE);
            }
            NodeStmtExit stmt_exit;
            if (auto node_expr = parse_expr()) {
                stmt_exit = { .expr = node_expr.value() };
            } else {
                std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Expected integer exit code" << std::endl;
                std::cerr << "compilation terminated." << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peak().has_value() && peak().value().type == TokenType::close_bracket) {
                consume();
            } else {
                std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Expected ')'" << std::endl;
                std::cerr << "compilation terminated." << std::endl;
                exit(EXIT_FAILURE);
            }
            if (peak().has_value() && peak().value().type == TokenType::semi) {
                consume();
            } else {
                std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Expected ';'" << std::endl;
                std::cerr << "compilation terminated." << std::endl;
                exit(EXIT_FAILURE);
            }
            return NodeStmt{ .var = stmt_exit };
        } else if (peak().has_value() && peak().value().type == TokenType::_int) {
            consume();
            if (peak().value().type == TokenType::ident) {
                m_index++;
                if (peak().value().type == TokenType::assign) {
                    m_index--;
                    auto stmt_int = NodeStmtInt{.ident = consume()};
                    consume();
                    if (auto expr = parse_expr()) {
                        stmt_int.expr = expr.value();
                    } else {
                        std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Invalid expression" << std::endl;
                        std::cerr << "compilation terminated." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    if (peak().has_value() && peak().value().type == TokenType::semi) {
                        consume();
                    } else {
                        std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Expected ':'" << std::endl;
                        std::cerr << "compilation terminated." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    return NodeStmt {.var = stmt_int};
                }
                m_index--;
                // Expected =
                std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Expected '=' or ';'" << std::endl;
                std::cerr << "compilation terminated." << std::endl;
                exit(EXIT_FAILURE);
            }
            // Expected identifier or integer
            std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Expected value" << std::endl;
            std::cerr << "compilation terminated." << std::endl;
            exit(EXIT_FAILURE);
        } else {
            return {};
        }
    }

    std::optional<NodeProg> parse_prog()
    {
        NodeProg prog;
        while (peak().has_value()) {
            if (auto stmt = parse_stmt()) {
                prog.stmts.push_back(stmt.value());
            }
            else {
                std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Invalid statement in program" << std::endl;
                std::cerr << "compilation terminated." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog;
    }
    
private:
    [[nodiscard]] inline std::optional<Token> peak(int offset = 1) const
    {
        if (m_index + offset > m_tokens.size()) {
            return {};
        } else {
            return m_tokens.at(m_index);
        }
    }

    inline Token consume()
    {
        return m_tokens.at(m_index++);
    }
    const std::vector<Token> m_tokens;
    size_t m_index = 0;
};