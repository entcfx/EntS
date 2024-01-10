#include <string>
#include <vector>
#include <memory>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <iostream>
#include "ast.h"
#include "ansi.h"
#include "token.h"

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), pos(0) {}

    std::unique_ptr<NodeProgram> parse() {
        return parseProgram();
    }

private:
    std::vector<Token> tokens;
    int pos = 0;

    std::unique_ptr<Node> parseProgram() {
        std::vector<std::unique_ptr<Node>> statements;
        while (pos < tokens.size()) {
            statements.push_back(parseSTMT());
        }
        return std::make_unique<NodeProgram>(
            "PROGRAM",
            std::move(statements)
            );
    }
    
    std::unique_ptr<Node> parseSTMT() {
        while (!isEOT()) {
            if (match(HEADER)) {
                return parseHeader();
            } else if (match(TYPEDEF)) {
                return parseTypedef();
            } else if (match(FUNC)) {
                return parseFunction();
            } else {
                error("expected a statement in the program");
            }
        }
    }

    std::unique_ptr<Node> parseHeader() {
        std::string name = consume().value.value();
        expect(HEADER);
        expect(OCB);
        std::vector<NodePrototype> prototypes;
        while (!match(CCB)) {
            prototypes.push_back(parsePrototype());
        }
        expect(CCB);
        return std::make_unique<NodeHeader>(name, std::move(prototypes));
    }

    std::unique_ptr<NodePrototype> parsePrototype() {
        expect(FUNC);
        std::string name = consume().value.value();
        expect(ORB);
        NodeFuncParams args;
        while (!match(CCB)) {
            NodeFuncParam param = parseFuncParam();
            args.push_back(param);
            if (match(COMMA)) {
                consume();
            }
        }
        expect(CCB);
        return std::make_unique<NodePrototype>(name, std::move(args));
    }

    // Helper functions
    Token peak(int offset = 0) {
        if (isEOT(offset)) {
            error("unexpected end of token stream");
        }
        return tokens[pos + offset];
    }

    Token consume() {
        if (isEOT()) {
            error("unexpected end of token stream");
        }
        return tokens[pos++];
    }

    bool match(int token) {
        if (peak().type == token) {
            return true;
        }
        return false;
    }

    void expect(int token) {
        if (peak().type == token) {
            consume();
            return;
        }
        error("expected token '" + std::to_string(token) + "'");
    }
    
    bool isEOT(int offset = 0) {
        int index = pos + offset;
        if (index >= tokens.size()) {
            return true;
        }
        Token token = tokens[index];
        return token.type == EOT;
    }

    void error(const std::string& message) {
        std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR
                << message << " at token '" << (pos < tokens.size() ? tokens[pos].value.value() : "end of file")
                << "' in position " << pos << std::endl;
        std::cerr << "compilation terminated." << std::endl;
        exit(EXIT_FAILURE);
    }
};