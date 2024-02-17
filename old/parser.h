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

    std::unique_ptr<Node> parseProgram()
    {
        std::vector<std::unique_ptr<Node>> statements;
        while (!isEOT())
        {
            statements.push_back(parseSTMT());
        }
        return std::make_unique<Node>(
            "PROGRAM",
            std::move(statements));
    }

    std::unique_ptr<Node> parseSTMT()
    {
        if (match(HEADER))
        {
            return parseHeader();
        }
        else if (match(TYPEDEF))
        {
            return parseTypedef();
        }
        else if (match(FUNC))
        {
            return parseFunction();
        }
        else
        {
            error("expected a statement in the program");
        }
        error("somehow reached end of parseSTMT?");
        // Return to make GCC happy
        return NULL;
    }

    std::unique_ptr<Node> parseHeader()
    {
        expect(HEADER);
        expect(OCB);
        std::vector<std::unique_ptr<Node>> prototypes;
        while (!match(CCB))
        {
            prototypes.push_back(parsePrototype());
        }
        expect(CCB);
        return std::make_unique<Node>("HEADER", std::move(prototypes));
    }

    std::unique_ptr<Node> parsePrototype()
    {
        std::vector<std::unique_ptr<Node>> args;
        expect(FUNC);
        std::string name = consume().value.value();
        expect(ORB);
        std::unique_ptr<Node> params = parseFuncParams();
        args.push_back(std::move(params));
        expect(CRB);
        // Body {}
        expect(OCB);
        expect(CCB);
        // Return type
        std::unique_ptr<Node> returnType = parseReturnType();
        args.push_back(std::move(returnType));
        expect(SEMI);
        return std::make_unique<Node>("FUNCTION_PROTOTYPE", std::move(args), std::optional<std::string>(name));
    }

    std::unique_ptr<Node> parseTypedef()
    {
        std::vector<std::unique_ptr<Node>> args;
        expect(TYPEDEF);
        std::unique_ptr<Node> oldValue = parseExpression();
        std::string type = consume().value.value();
        expect(SEMI);
        args.push_back(std::move(oldValue));
        return std::make_unique<Node>("TYPEDEF", std::move(args), std::optional<std::string>(type));
    }

    std::unique_ptr<Node> parseFuncParams()
    {
        std::vector<std::unique_ptr<Node>> args;
        while (!match(CRB))
        {
            std::unique_ptr<Node> param = parseFuncParam();
            args.push_back(std::move(param));
            if (match(COMMA))
            {
                consume();
            }
        }
        std::unique_ptr<Node> paramsz = std::make_unique<Node>("PARAMS", std::move(args));
        return paramsz;
    }

    std::unique_ptr<Node> parseFunction()
    {
        std::vector<std::unique_ptr<Node>> args;
        expect(FUNC);
        std::string name = consume().value.value();
        expect(ORB);
        std::unique_ptr<Node> params = parseFuncParams();
        args.push_back(std::move(params));
        expect(CRB);
        expect(OCB);
        std::unique_ptr<Node> body = parseBody();
        args.push_back(std::move(body));
        expect(CCB);
        std::unique_ptr<Node> returnType = parseReturnType();
        args.push_back(std::move(returnType));
        expect(SEMI);
        return std::make_unique<Node>("FUNCTION", std::move(args), std::optional<std::string>(name));
        return NULL;
    }

    std::unique_ptr<Node> parseFuncParam()
    {
        std::vector<std::unique_ptr<Node>> args;
        //  First, parse the type. It can be a keyword (CHAR, INT8, INT16, INT32, INT64, VOID, UINT8, UINT16) or
        // A identifier which must be in std::vector<std::string> existing_types; declared in ast.h
        Token type = consume();
        if (type.type == UINT8 || type.type == UINT16 || type.type == UINT32 || type.type == UINT64 ||
            type.type == INT8 || type.type == INT16 || type.type == INT32 || type.type == INT64 || type.type == CHAR ||
            type.type == VOIDD)
        {
            args.push_back(std::make_unique<Node>("TYPE", std::optional<std::string>(type.value.value())));
        }
        else if (std::find(existing_types.begin(), existing_types.end(), type.value.value()) != existing_types.end())
        {
            args.push_back(std::make_unique<Node>("TYPE", std::optional<std::string>(type.value.value())));
        }
        else
        {
            error("expected a type");
        }
        Token name = consume();
        if (name.type == IDENTIFIER)
        {
            args.push_back(std::make_unique<Node>("NAME", std::optional<std::string>(name.value.value())));
        }
        else
        {
            error("expected a name");
        }

        if (match(ASSIGN))
        {
            consume();
            std::unique_ptr<Node> value = parseExpression();
            args.push_back(std::move(value));
        }
        // Add the new variable into std::vector<std::string> existing_variables;
        existing_variables.push_back(name.value.value());
        return std::make_unique<Node>("PARAM", std::move(args));
        return NULL;
    }

    std::unique_ptr<Node> parseReturnType()
    {
        // First, parse the type. It can be a keyword (CHAR, INT8, INT16, INT32, INT64, VOID, UINT8, UINT16) or identifier in std::vector<std::string> existing_types;
        Token type = consume();
        if (type.type == UINT8 || type.type == UINT16 || type.type == UINT32 || type.type == UINT64 ||
            type.type == INT8 || type.type == INT16 || type.type == INT32 || type.type == INT64 || type.type == CHAR ||
            type.type == VOIDD)
        {
            return std::make_unique<Node>("TYPE", std::optional<std::string>(type.value.value()));
        }
        else if (std::find(existing_types.begin(), existing_types.end(), type.value.value()) != existing_types.end())
        {
            return std::make_unique<Node>("TYPE", std::optional<std::string>(type.value.value()));
        }
        else
        {
            error("expected a type");
        }
        return NULL;
    }

    std::unique_ptr<Node> parseExpression()
    {
        return parseEquality();
    }

    std::unique_ptr<Node> parseEquality()
    {
        // Implementation for parsing equality expressions (==, !=)

        // Check if there is a == or != anywhere in the expression
        // If there is, split the expression into two parts and parse them separately

        return parseComparison();
    }

    std::unique_ptr<Node> parseComparison()
    {
        // Implementation for parsing comparison expressions (<, >, <=, >=)

        return parseAddition();
    }

    std::unique_ptr<Node> parseAddition()
    {
        // Implementation for parsing addition and subtraction

        return parseMultiplication();
    }

    std::unique_ptr<Node> parseMultiplication()
    {
        // Implementation for parsing multiplication and division

        return parseUnary();
    }

    std::unique_ptr<Node> parseUnary()
    {
        // Implementation for parsing unary operators (like -, !)

        return parsePrimary();
    }

    std::unique_ptr<Node> parsePrimary()
    {
        // Implementation for parsing primary expressions (numbers, parentheses)
        // This is the base case for recursion
    }

    std::unique_ptr<Node> parseBody()
    {
        // TODO
        return NULL;
    }

    // Helper functions
    Token peak(int offset = 0)
    {
        if (isEOT(offset))
        {
            error("unexpected end of token stream");
        }
        return tokens[pos + offset];
    }

    Token consume()
    {
        if (isEOT())
        {
            error("unexpected end of token stream");
        }
        return tokens[pos++];
    }

    bool match(int token)
    {
        if (peak().type == token)
        {
            return true;
        }
        return false;
    }

    void expect(int token)
    {
        if (peak().type == token)
        {
            consume();
            return;
        }
        error("expected token '" + std::to_string(token) + "'");
    }

    bool isEOT(int offset = 0)
    {
        int index = pos + offset;
        if (index >= tokens.size())
        {
            return true;
        }
        Token token = tokens[index];
        return token.type == EOT;
    }

    void error(const std::string &message)
    {
        std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR
                  << message << " at token '" << (pos < tokens.size() ? tokens[pos].value.value() : "end of file")
                  << "' in position " << pos << std::endl;
        std::cerr << "compilation terminated." << std::endl;
        exit(EXIT_FAILURE);
    }
};