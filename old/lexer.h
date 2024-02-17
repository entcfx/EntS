#pragma once
#include <optional>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "ansi.h"
#include "token.h"

#define INC 12

class Tokenizer
{
public:
    inline explicit Tokenizer(const std::string src)
        : m_src(std::move(src))
    {
    }

    std::vector<Token> tokenize()
    {
        std::string buffer;
        std::vector<Token> tokens;

        while (peak().has_value())
        {
            // TODO Comments
            //  First, if we find two //, we will skip the entire rest of the line

            if (peak().value() == '/' && m_src.at(m_index + 1) == '*')
            {
                consume();
                consume();
                // Consume until new line
                while (peak().has_value() && peak().value() != '\n')
                {
                    consume();
                }
                continue;
            }

            // Whitespace
            if (std::isspace(peak().value()))
            {
                consume();
                continue;
            }

            // May be IDENTIFIER or KEYWORD
            if (std::isalpha(peak().value()))
            {
                buffer.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value()))
                {
                    buffer.push_back(consume());
                }
                std::string oldbuffer = buffer;
                // Convert to lowercase
                std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);

                // Check for KEYWORDS
                if (oldbuffer == "HEADER")
                { // Special header block like .h for C
                    tokens.push_back({.type = HEADER});
                    buffer.clear();
                    continue;
                }
                if (buffer == "break")
                {
                    tokens.push_back({.type = BREAK});
                    buffer.clear();
                    continue;
                }
                if (buffer == "case")
                {
                    tokens.push_back({.type = CASE});
                    buffer.clear();
                    continue;
                }
                if (buffer == "char")
                {
                    tokens.push_back({.type = CHAR});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int8")
                {
                    tokens.push_back({.type = INT8});
                    buffer.clear();
                    continue;
                }
                if (buffer == "continue")
                {
                    tokens.push_back({.type = CONTIN});
                    buffer.clear();
                    continue;
                }
                if (buffer == "default")
                {
                    tokens.push_back({.type = DEFAULT});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int16")
                {
                    tokens.push_back({.type = INT16});
                    buffer.clear();
                    continue;
                }
                if (buffer == "else")
                {
                    tokens.push_back({.type = ELSE});
                    buffer.clear();
                    continue;
                }
                if (buffer == "extern")
                {
                    tokens.push_back({.type = EXTERN});
                    buffer.clear();
                    continue;
                }
                if (buffer == "nop")
                {
                    tokens.push_back({.type = NOP});
                    buffer.clear();
                    continue;
                }
                if (buffer == "for")
                {
                    tokens.push_back({.type = FOR});
                    buffer.clear();
                    continue;
                }
                if (buffer == "goto")
                {
                    tokens.push_back({.type = GOTO});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int32")
                {
                    tokens.push_back({.type = INT32});
                    buffer.clear();
                    continue;
                }
                if (buffer == "if")
                {
                    tokens.push_back({.type = IF});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int")
                {
                    tokens.push_back({.type = INT});
                    buffer.clear();
                    continue;
                }
                if (buffer == "byout")
                {
                    tokens.push_back({.type = BYTEOUT});
                    buffer.clear();
                    continue;
                }
                if (buffer == "return")
                {
                    tokens.push_back({.type = RETURN});
                    buffer.clear();
                    continue;
                }
                if (buffer == "sizeof")
                {
                    tokens.push_back({.type = SIZEOF});
                    buffer.clear();
                    continue;
                }
                if (buffer == "function")
                {
                    tokens.push_back({.type = FUNC});
                    buffer.clear();
                    continue;
                }
                if (buffer == "struct")
                {
                    tokens.push_back({.type = STRUC});
                    buffer.clear();
                    continue;
                }
                if (buffer == "switch")
                {
                    tokens.push_back({.type = SWITCH});
                    buffer.clear();
                    continue;
                }
                if (buffer == "typedef")
                {
                    tokens.push_back({.type = TYPEDEF});
                    buffer.clear();
                    continue;
                }
                if (buffer == "reg")
                {
                    tokens.push_back({.type = REG});
                    buffer.clear();
                    continue;
                }
                if (buffer == "byin")
                {
                    tokens.push_back({.type = BYTEIN});
                    buffer.clear();
                    continue;
                }
                if (buffer == "void")
                {
                    tokens.push_back({.type = VOIDD});
                    buffer.clear();
                    continue;
                }
                if (buffer == "while")
                {
                    tokens.push_back({.type = WHILE});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int64")
                {
                    tokens.push_back({.type = INT64});
                    buffer.clear();
                    continue;
                }
                // It is a identifier
                else
                {
                    tokens.push_back({.type = IDENTIFIER, .value = buffer});
                    buffer.clear();
                    continue;
                }
            }

            // INTEGER
            else if (std::isdigit(peak().value()))
            {
                int num_type;
                buffer.push_back(consume());
                if (peak().value() == 'x')
                {
                    buffer.push_back(consume());
                    num_type = 16;
                }
                else if (peak().value() == 'b')
                {
                    buffer.push_back(consume());
                    num_type = 2;
                }
                else if (peak().value() == 'o')
                {
                    buffer.push_back(consume());
                    num_type = 8;
                }
                else if (peak().value() == 'd')
                {
                    buffer.push_back(consume());
                    num_type = 10;
                }
                while (peak().has_value() && std::isdigit(peak().value()))
                {
                    buffer.push_back(consume());
                }

                if (num_type == 16)
                    tokens.push_back({.type = HEX, .value = buffer});
                else if (num_type == 2)
                    tokens.push_back({.type = BINARY, .value = buffer});
                else if (num_type == 8)
                    tokens.push_back({.type = OCTAL, .value = buffer});
                else if (num_type == 10)
                    tokens.push_back({.type = DECIM, .value = buffer});

                buffer.clear();
                continue;
            }

            // SEMICOLON
            else if (peak().value() == ';')
            {
                consume();
                tokens.push_back({.type = SEMI});
                buffer.clear();
                continue;
            }

            // Open / Close brackets ()
            else if (peak().value() == '(')
            {
                consume();
                tokens.push_back({.type = ORB});
                buffer.clear();
                continue;
            }
            else if (peak().value() == ')')
            {
                consume();
                tokens.push_back({.type = CRB});
                buffer.clear();
                continue;
            }

            // Assign =
            else if (peak().value() == '=')
            {
                consume();
                tokens.push_back({.type = ASSIGN});
                buffer.clear();
                continue;
            }
            // Open block { Close block }
            else if (peak().value() == '{')
            {
                consume();
                tokens.push_back({.type = OCB});
                buffer.clear();
                continue;
            }
            else if (peak().value() == '}')
            {
                consume();
                tokens.push_back({.type = CCB});
                buffer.clear();
                continue;
            }
            else if (peak().value() == '[')
            {
                consume();
                tokens.push_back({.type = OSB});
                buffer.clear();
                continue;
            }
            else if (peak().value() == ']')
            {
                consume();
                tokens.push_back({.type = CSB});
                buffer.clear();
                continue;
            }
            // Comparisons: != == > < >= <=
            if (peak().value() == '!')
            {
                consume();
                if (peak().has_value() && (peak().value() != '='))
                {
                    tokens.push_back({.type = NOT});
                    buffer.clear();
                    continue;
                }
                else if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = NE});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '>')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = GE});
                    buffer.clear();
                    continue;
                }
                else if (peak().has_value() && (peak().value() == '>'))
                {
                    consume();
                    tokens.push_back({.type = SHR});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = GT});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '<')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = LE});
                    buffer.clear();
                    continue;
                }
                else if (peak().has_value() && (peak().value() == '<'))
                {
                    consume();
                    tokens.push_back({.type = SHL});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = LT});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '+')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = ADDE});
                    buffer.clear();
                    continue;
                }
                else if (peak().has_value() && (peak().value() == '+'))
                {
                    consume();
                    tokens.push_back({.type = INC});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = ADD});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '-')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = SUBE});
                    buffer.clear();
                    continue;
                }
                else if (peak().has_value() && (peak().value() == '-'))
                {
                    consume();
                    tokens.push_back({.type = DEC});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = SUB});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '*')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = STARE});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = MUL});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '/')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = DIVE});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = DIV});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '&')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = ANDE});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = AND});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '|')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = ORE});
                    buffer.clear();
                    continue;
                }
                else if (peak().has_value() && (peak().value() == '|'))
                {
                    consume();
                    tokens.push_back({.type = DOR});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = OR});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '^')
            {
                consume();
                if (peak().has_value() && (peak().value() == '='))
                {
                    consume();
                    tokens.push_back({.type = XORE});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = XOR});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().value() == '~')
            {
                consume();
                tokens.push_back({.type = COM});
                buffer.clear();
                continue;
            }
            if (peak().has_value() && peak().value() == '%')
            {
                consume();
                if (peak().has_value() && peak().value() == '=')
                {
                    consume();
                    tokens.push_back({.type = MODE});
                    buffer.clear();
                    continue;
                }
                else
                {
                    tokens.push_back({.type = MOD});
                    buffer.clear();
                    continue;
                }
            }

            if (peak().has_value() && peak().value() == ',')
            {
                consume();
                tokens.push_back({.type = COMMA});
                buffer.clear();
                continue;
            }
            if (peak().has_value() && peak().value() == '.')
            {
                consume();
                if (peak().has_value() && peak().value() == '.')
                {
                    consume();
                    if (peak().has_value() && peak().value() == '.')
                    {
                        consume();
                        tokens.push_back({.type = ELIPSE});
                        buffer.clear();
                        continue;
                    }
                    else
                    {
                        tokens.push_back({.type = DOT});
                        tokens.push_back({.type = DOT});
                        buffer.clear();
                        continue;
                    }
                }
                else
                {
                    tokens.push_back({.type = DOT});
                    buffer.clear();
                    continue;
                }
            }
            if (peak().has_value() && peak().value() == ':')
            {
                consume();
                tokens.push_back({.type = COLON});
                buffer.clear();
                continue;
            }

            // String Literal.
            // Started by a " and ended by a "
            if (peak().has_value() && peak().value() == '"')
            {
                consume();
                while (peak().has_value() && peak().value() != '"')
                {
                    buffer += consume();
                }
                if (peak().has_value() && peak().value() == '"')
                {
                    consume();
                    tokens.push_back({.type = STRING, .value = buffer});
                    buffer.clear();
                    continue;
                }
                else
                {
                    // Unterminated string literal.
                    std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR
                              << "unterminated string literal at position " << m_index << " with character '"
                              << peak().value() << "'" << std::endl;
                    std::cerr << "compilation terminated." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            // Floating point numbers (only decimal allowed)
            if (peak().has_value() && std::isdigit(peak().value()))
            {
                while (peak().has_value() && std::isdigit(peak().value()))
                {
                    buffer += consume();
                }
                if (peak().has_value() && peak().value() == '.')
                {
                    consume();
                    while (peak().has_value() && std::isdigit(peak().value()))
                    {
                        buffer += consume();
                    }
                }
                tokens.push_back({.type = DECIMFLOAT, .value = buffer});
                buffer.clear();
                continue;
            }

            // Started by a ' and ended by a '
            if (peak().has_value() && peak().value() == '\'')
            {
                consume();
                if (peak().has_value() && peak().value() == '\\')
                {
                    consume();
                    buffer += consume();

                    tokens.push_back({.type = CHARLIT, .value = buffer});

                    if (peak().has_value() && peak().value() == '\'')
                        consume();
                    else
                    {
                        // Unterminated char literal.
                        std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR
                                  << "unterminated char literal at position " << m_index << " with character '"
                                  << peak().value() << "'" << std::endl;
                        std::cerr << "compilation terminated." << std::endl;
                        exit(EXIT_FAILURE);
                    }

                    buffer.clear();
                    continue;
                }
                else if (peak().has_value() && peak().value() == '\'')
                {
                    consume();
                    tokens.push_back({.type = CHARLIT});

                    buffer.clear();
                    continue;
                }
                else
                {
                    buffer += consume();
                    tokens.push_back({.type = CHARLIT, .value = buffer});
                    if (peak().has_value() && peak().value() == '\'')
                        consume();
                    else
                    {
                        // Unterminated char literal.
                        std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR
                                  << "unterminated char literal at position " << m_index << " with character '"
                                  << peak().value() << "'" << std::endl;
                        std::cerr << "compilation terminated." << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    buffer.clear();
                    continue;
                }
            }

            // Unknown character.
            std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "unknown character in lexer at position " << m_index << " with character '" << peak().value() << "'" << std::endl;

            std::cerr << "compilation terminated." << std::endl;
            exit(EXIT_FAILURE);
        }

        m_index = 0;
        tokens.push_back({.type = EOT});
        return tokens;
    }

private:
    [[nodiscard]] inline std::optional<char> peak(int offset = 1) const
    {
        if (m_index + offset > m_src.length())
        {
            return {};
        }
        else
        {
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