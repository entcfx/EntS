#pragma once
#include <optional>
#include <string>
#include <vector>
#include "ansi.h"
#include "token.h"
// IDENTIFIER
// KEYWORD: function, int, char, while, if, else, return, header, typedef
// INTEGER: 1, 2, 59, 0x15 (hex), 0xFF
// SEMICOLON: ;
// OPERATION: + - * / %
// ASSIGN: =
// EQUSATIONS: == != <= >= < >
// LOGICAL_OPERATORS: && || !
// BRACKET: ( )
// BLOCK: { }
// CHAR_STR: '

// MEMORY_LOCATION: [IDENTIFIER]
// POINTER: *IDENTIFIER
// EOF: End of File


class Tokenizer {
public:
    inline explicit Tokenizer(const std::string src)
        : m_src(std::move(src))
    {
    }


    std::vector<Token> tokenize()
    {
        std::string buffer;
        std::vector<Token> tokens;

        while (peak().has_value()) {
            //TODO Comments
            // First, if we find two //, we will skip the entire rest of the line
            // if (peak().value() == '/' && m_src.at(m_index + 1) == '/') {
            //     consume();
            //     consume();
            // }

            if (std::isspace(peak().value()))
            {
                consume();
                continue;
            }

            // May be IDENTIFIER or KEYWORD
            if (std::isalpha(peak().value())) {
                buffer.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value())) {
                    buffer.push_back(consume());
                }

                // Check for KEYWORDS
                if (buffer == "HEADER") { // Special header block like .h for C
                    tokens.push_back({.type = TokenType::HEADER});
                    buffer.clear();
                    break;
                }
                if (buffer == "exit") {
                    tokens.push_back({.type = TokenType::_exit});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int") {
                    tokens.push_back({.type = TokenType::_int});
                    buffer.clear();
                    continue;
                }
                if (buffer == "function") {
                    tokens.push_back({.type = TokenType::_func});
                    buffer.clear();
                    continue;
                }
                if (buffer == "while") {
                    tokens.push_back({.type = TokenType::_while});
                    buffer.clear();
                    continue;
                }
                if (buffer == "if") {
                    tokens.push_back({.type = TokenType::_if});
                    buffer.clear();
                    continue;
                }
                if (buffer == "else") {
                    tokens.push_back({.type = TokenType::_else});
                    buffer.clear();
                    continue;
                }
                if (buffer == "char") {
                    tokens.push_back({.type = TokenType::_char});
                    buffer.clear();
                    continue;
                }
                // It is a identifier
                else {
                    tokens.push_back({.type = TokenType::ident, .value = buffer});
                    buffer.clear();
                    continue;
                }
            }

            // INTEGER
            else if (std::isdigit(peak().value()))
            {
                while (peak().has_value() && std::isdigit(peak().value())) {
                    buffer.push_back(consume());
                }

                tokens.push_back({.type = TokenType::int_lit, .value = buffer});
                buffer.clear();
                continue;
            }


            // SEMICOLON
            else if (peak().value() == ';')
            {
                consume();
                tokens.push_back({.type = TokenType::semi});
                buffer.clear();
                continue;
            }
            
            // Open / Close brackets ()
            else if (peak().value() == '(')
            {
                consume();
                tokens.push_back({.type = TokenType::open_bracket});
                buffer.clear();
                continue;
            }
            else if (peak().value() == ')')
            {
                consume();
                tokens.push_back({.type = TokenType::close_bracket});
                buffer.clear();
                continue;
            }

            // Assign =
            else if (peak().value() == '=')
            {
                consume();
                tokens.push_back({.type = TokenType::assign});
                buffer.clear();
                continue;
            }
            // Operands + - * /
            //TODO Operands like + 

            // static const char* keywords[] = {"if", "else", "while", "return", "break", "continue", "HEADER", "typedef", "function", "int", "char", "uint"};

            std::cerr   << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR <<
                        "invalid occurrence in lexer at position " << m_index << " with character '" << peak().value() << "'" << std::endl;

            std::cerr << "compilation terminated." << std::endl;
            exit(EXIT_FAILURE);
        }
        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] inline std::optional<char> peak(int offset = 1) const
    {
        if (m_index + offset > m_src.length()) {
            return {};
        } else {
            return m_src.at(m_index);
        }
    }

    inline char consume()
    {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index = 0;
};