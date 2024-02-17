#include <string>
#include <vector>
#include <memory>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>

#include "ast.h"
#include "ansi.h"
#include "token.h"

class Parser
{
public:
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), pos(0) {}

    std::unique_ptr<Node> parse()
    {
        return parseProgram();
    }

private:
    std::vector<Token> tokens;
    int pos = 0;

    std::unique_ptr<Node> parseProgram();
    std::unique_ptr<Node> parseSTMT();
    std::unique_ptr<Node> parseHeader();
    std::unique_ptr<Node> parsePrototype();
    std::unique_ptr<Node> parseTypedef();
    std::unique_ptr<Node> parseFuncParams();
    std::unique_ptr<Node> parseFunction();
    std::unique_ptr<Node> parseFuncParam();
    std::unique_ptr<Node> parseReturnType();
    std::unique_ptr<Node> parseBody();
    std::unique_ptr<Node> parseStruct();

    std::unique_ptr<Node> parseExpression()
    {
        return parseEquality();
    }
    std::unique_ptr<Node> parseEquality();
    std::unique_ptr<Node> parseComparison();
    std::unique_ptr<Node> parseAddition();
    std::unique_ptr<Node> parseMultiplication();
    std::unique_ptr<Node> parseUnary();
    std::unique_ptr<Node> parsePrimary();

    // Helper functions
    Token peak(int offset = 0)
    {
        if (isEOT(offset))
        {
            errorf("unexpected end of token stream");
        }
        return tokens[pos + offset];
    }

    Token consume()
    {
        if (isEOT())
        {
            errorf("unexpected end of token stream");
        }
        return tokens[pos++];
    }

    bool match(TokenType token)
    {
        if (peak().type == token)
        {
            return true;
        }
        return false;
    }

    void expect(TokenType token)
    {
        if (peak().type == token)
        {
            consume();
            return;
        }
        errorf("expected token '%s'\n", token_type_to_string(token).c_str());
    }

    bool isEOT(int offset = 0)
    {
        int index = pos + offset;
        if (index >= tokens.size())
        {
            return true;
        }
        Token token = tokens[index];
        return token.type == TokenType::TkEof;
    }
};