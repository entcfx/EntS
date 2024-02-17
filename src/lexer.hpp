#pragma once
#include <optional>
#include <iostream>
#include <vector>

#include "token.h"
#include "ansi.h"

class Tokenizer
{
public:
    inline explicit Tokenizer(const std::string src)
        : m_src(std::move(src))
    {
    }

    std::vector<Token> tokenize()
    {
        bool oneError = false;
        std::string buffer;
        std::vector<Token> tokens;

        while (peak().has_value())
        {
            if (peak().value() == '/' && peak(1).has_value() && peak(1).value() == '/')
            {
                consume();
                consume();
                while (peak().has_value() && peak().value() != '\n')
                {
                    consume();
                }
                m_line++;
                continue;
            }
            if (peak().value() == '/' && peak(1).has_value() && peak(1).value() == '*')
            {
                consume();
                consume();
                while (peak().has_value() && peak().value() != '*' && peak(1).has_value() && peak(1).value() != '/')
                {
                    if (peak().value() == '\n')
                        m_line++;
                    consume();
                }
                if (peak().has_value())
                {
                    consume();
                    consume();
                }
                else
                {
                    std::string line = GetLine();
                    char symbol = peak().value();
                    errorf("unterminated comment \'%c\' at position \"%d\" at line %d:\n%s", symbol, m_index, m_line, line.c_str());
                }
                continue;
            }
            if (peak().value() == '\n')
            {
                m_line++;
            }

            if (std::isalpha(peak().value()))
            {
                buffer.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value()))
                {
                    buffer.push_back(consume());
                }

                if (buffer == "HEADER")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Header});
                    buffer.clear();
                    continue;
                }
                if (buffer == "break")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Break});
                    buffer.clear();
                    continue;
                }
                if (buffer == "case")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Case});
                    buffer.clear();
                    continue;
                }
                if (buffer == "continue")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Continue});
                    buffer.clear();
                    continue;
                }
                if (buffer == "default")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Default});
                    buffer.clear();
                    continue;
                }
                if (buffer == "else")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Else});
                    buffer.clear();
                    continue;
                }
                if (buffer == "extern")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Extern});
                    buffer.clear();
                    continue;
                }
                if (buffer == "for")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::For});
                    buffer.clear();
                    continue;
                }
                if (buffer == "if")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::If});
                    buffer.clear();
                    continue;
                }
                if (buffer == "return")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Return});
                    buffer.clear();
                    continue;
                }
                if (buffer == "sizeof")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Sizeof});
                    buffer.clear();
                    continue;
                }
                if (buffer == "function")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Function});
                    buffer.clear();
                    continue;
                }
                if (buffer == "struct")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Struct});
                    buffer.clear();
                    continue;
                }
                if (buffer == "switch")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Switch});
                    buffer.clear();
                    continue;
                }
                if (buffer == "typedef")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Typedef});
                    buffer.clear();
                    continue;
                }
                if (buffer == "while")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::While});
                    buffer.clear();
                    continue;
                }

                if (buffer == "void")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Void});
                    buffer.clear();
                    continue;
                }
                if (buffer == "char")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Char});
                    buffer.clear();
                    continue;
                }
                if (buffer == "uint8")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::uint8});
                    buffer.clear();
                    continue;
                }
                if (buffer == "uint16")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::uint16});
                    buffer.clear();
                    continue;
                }
                if (buffer == "uint32")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::uint32});
                    buffer.clear();
                    continue;
                }
                if (buffer == "uint64")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::uint64});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int8")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::int8});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int16")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::int16});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int32")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::int32});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int64")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::int64});
                    buffer.clear();
                    continue;
                }
                if (buffer == "int")
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::Int}); // Interrupt, not integer
                    buffer.clear();
                    continue;
                }
                else
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::_Identifier,
                                      .value = buffer});
                    buffer.clear();
                    continue;
                }
            }
            else if (std::isdigit(peak().value()))
            {
                int num_type;
                int _floatingPoint;
                buffer.push_back(consume());
                switch (peak().value())
                { // TODO This wokrs but shouldnt: "0k9485", only gets caught by assembler
                case 'x':
                    buffer.push_back(consume());
                    num_type = 16;
                case 'b':
                    buffer.push_back(consume());
                    num_type = 2;
                case 'd':
                    buffer.push_back(consume());
                    num_type = 10;
                default:
                    buffer.push_back(consume());
                    num_type = 10;
                }
                if (num_type == 10)
                {
                    while (peak().has_value() && (std::isdigit(peak().value()) || peak().value() == '.'))
                    {
                        if (peak().value() == '.')
                        {
                            if (_floatingPoint)
                            {
                                errorf("Invalid floating-point literal with multiple periods at line %d", m_line);
                            }
                            _floatingPoint = true;
                        }
                        buffer.push_back(consume());
                    }
                }
                else
                {
                    while (peak().has_value() && std::isdigit(peak().value()))
                    {
                        buffer.push_back(consume());
                    }
                }
                if (_floatingPoint)
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::_DecimFloat, .value = buffer});
                }
                else if (num_type == 16)
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::_Hex, .value = buffer});
                }
                else if (num_type == 2)
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::_Binary, .value = buffer});
                }
                else if (num_type == 10)
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::_Decimal, .value = buffer});
                }
                buffer.clear();
                continue;
            }
            if (peak().value() == '>')
            {
                consume();
                if (peak().has_value() && peak().value() == '=')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::GreaterEqual});
                    continue;
                }
                else if (peak().has_value() && peak().value() == '>')
                {
                    consume();
                    if (peak().has_value() && peak().value() == '=')
                    {
                        consume();
                        oneError = false;
                        tokens.push_back({.type = TokenType::ShiftRightAssign});
                    }
                    else
                    {
                        oneError = false;
                        tokens.push_back({.type = TokenType::ShiftRight});
                    }
                    continue;
                }
            }
            if (peak().value() == '<')
            {
                consume();
                if (peak().has_value() && peak().value() == '=')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::LessEqual});
                    continue;
                }
                else if (peak().has_value() && peak().value() == '<')
                {
                    consume();
                    if (peak().has_value() && peak().value() == '=')
                    {
                        consume();
                        oneError = false;
                        tokens.push_back({.type = TokenType::ShiftLeftAssign});
                    }
                    else
                    {
                        oneError = false;
                        tokens.push_back({.type = TokenType::ShiftLeft});
                    }
                    continue;
                }
            }

            if (peak().value() == '!' && peak(1).has_value() && peak(1).value() == '=')
            {
                consume();
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::NotEqual});
                continue;
            }
            else if (peak().value() == '=' && peak(1).has_value() && peak(1).value() == '=')
            {
                consume();
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Equal});
                continue;
            }
            else if (peak().value() == '+')
            {
                consume();
                if (peak().has_value() && peak().value() == '=')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::PlusEqual});
                    continue;
                }
                else if (peak().has_value() && peak().value() == '+')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::Increment});
                    continue;
                }
            }
            else if (peak().value() == '-')
            {
                consume();
                if (peak().has_value() && peak().value() == '=')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::MinusEqual});
                    continue;
                }
                else if (peak().has_value() && peak().value() == '-')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::Decrement});
                    continue;
                }
                else if (peak().has_value() && peak().value() == '>')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::Arrow});
                    continue;
                }
            }
            else if (peak().value() == '&')
            {
                consume();
                if (peak().value() == '&')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::BitAnd});
                    continue;
                }
                else if (peak().value() == '=')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::AndEqual});
                    continue;
                }
            }
            else if (peak().value() == '|')
            {
                consume();
                if (peak().value() == '|')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::BitOr});
                    continue;
                }
                else if (peak().value() == '=')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::OrEqual});
                    continue;
                }
            }
            if (peak().value() == '*' && peak(1).has_value() && (peak(1).value() == '='))
            {
                consume();
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::StarEqual});
                continue;
            }
            if (peak().value() == '/' && peak(1).has_value() && (peak(1).value() == '='))
            {
                consume();
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::DivEqual});
                continue;
            }
            if (peak().value() == '%' && peak(1).has_value() && (peak(1).value() == '='))
            {
                consume();
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::ModEqual});
                continue;
            }
            if (peak().value() == '^' && peak(1).has_value() && (peak(1).value() == '='))
            {
                consume();
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::XorEqual});
                continue;
            }

            if (peak().value() == ';')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Semicolon});
                buffer.clear();
                continue;
            }
            if (peak().value() == '(')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::OpenBracket});
                buffer.clear();
                continue;
            }
            if (peak().value() == ')')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::CloseBracket});
                buffer.clear();
                continue;
            }
            if (peak().value() == '!')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Not});
                buffer.clear();
                continue;
            }
            if (peak().value() == '%')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Modular});
                buffer.clear();
                continue;
            }
            if (peak().value() == '&')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::And});
                buffer.clear();
                continue;
            }
            if (peak().value() == '*')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Star});
                buffer.clear();
                continue;
            }
            if (peak().value() == '+')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Plus});
                buffer.clear();
                continue;
            }
            if (peak().value() == ',')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Comma});
                buffer.clear();
                continue;
            }
            if (peak().value() == '-')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Minus});
                buffer.clear();
                continue;
            }
            if (peak().value() == '.')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Dot});
                buffer.clear();
                continue;
            }
            if (peak().value() == '/')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Div});
                buffer.clear();
                continue;
            }
            if (peak().value() == ':')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Colon});
                buffer.clear();
                continue;
            }
            if (peak().value() == '<')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Less});
                buffer.clear();
                continue;
            }
            if (peak().value() == '=')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Assign});
                buffer.clear();
                continue;
            }
            if (peak().value() == '>')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Greater});
                buffer.clear();
                continue;
            }
            if (peak().value() == '?')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Question});
                buffer.clear();
                continue;
            }
            if (peak().value() == '[')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::OpenSquare});
                buffer.clear();
                continue;
            }
            if (peak().value() == ']')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::CloseSquare});
                buffer.clear();
                continue;
            }
            if (peak().value() == '^')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Xor});
                buffer.clear();
                continue;
            }
            if (peak().value() == '{')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::OpenCurly});
                buffer.clear();
                continue;
            }
            if (peak().value() == '}')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::CloseCurly});
                buffer.clear();
                continue;
            }
            if (peak().value() == '|')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Or});
                buffer.clear();
                continue;
            }
            if (peak().value() == '~')
            {
                consume();
                oneError = false;
                tokens.push_back({.type = TokenType::Tilde});
                buffer.clear();
                continue;
            }

            if (peak().has_value() && peak().value() == '"')
            {
                consume();

                if (!peak().has_value())
                {
                    errorf("Unclosed string literal at line %d", m_line);
                    continue;
                }

                while (peak().has_value() && peak().value() != '"' && peak().value() != '\n')
                {
                    buffer += consume();
                }

                if (peak().has_value() && peak().value() == '"')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::_String, .value = buffer});
                    buffer.clear();
                    continue;
                }
                else
                {
                    std::string line = GetLine();
                    char symbol = peak().value();
                    errorf("unterminated string literal \'%c\' at position \"%d\" at line %d:\n%s", symbol, m_index, m_line, line.c_str());
                }
            }

            if (peak().has_value() && peak().value() == '\'') // TODO Implement error handling when more than one letter in a char lit
            {
                consume();

                if (!peak().has_value())
                {
                    errorf("Unclosed character literal at line %d", m_line);
                    continue;
                }

                std::string charValue;
                if (peak().value() == '\\')
                {
                    charValue += consume();
                    if (peak().has_value())
                    {
                        charValue += consume();
                    }
                    else
                    {
                        errorf("Incomplete escape sequence in char literal at line %d", m_line);
                        continue;
                    }
                }
                else if (peak().value() == '\'')
                {
                    oneError = false;
                    tokens.push_back({.type = TokenType::_CharLit}); // Null char lit
                }
                else
                {
                    charValue = consume();
                }

                if (peak().has_value() && peak().value() == '\'')
                {
                    consume();
                    oneError = false;
                    tokens.push_back({.type = TokenType::_CharLit, .value = charValue});
                }
                else
                {
                    // Error: missing closing quote
                    errorf("Unterminated char literal at line %d", m_line);
                }
            }

            if (peak().has_value() && std::isspace(peak().value()))
            {
                consume();
                continue;
            }

            if (!oneError)
            {
                oneError = true;
                // std::cerr << "Warning: Problematic token encountered. Attempting to recover...\n This is most likely not your fault so ignore this if there isnt an error under it!" << std::endl;
                // User doesnt care ngl
                continue;
            }
            else
            {
                // Error, invalid token
                std::string line = GetLine();
                char symbol = peak().value();
                errorf("invalid symbol \'%c\' in lexer at index \"%d\" at line %d:\n%s", symbol, m_index, m_line, line.c_str());
            }
        }
        tokens.push_back(Token{TokenType::TkEof});
        return tokens;
    }

private:
    [[nodiscard]] inline std::optional<char> peak(int offset = 0) const
    {
        size_t peekIndex = m_index + offset;
        if (peekIndex >= m_src.length())
        {
            return std::nullopt;
        }
        else
        {
            return m_src.at(peekIndex);
        }
    }

    std::string GetLine()
    {
        size_t lineStart = 0;
        size_t currentLine = 0;
        std::string line;

        for (size_t i = 0; i <= m_src.length(); ++i)
        {
            if (i == m_src.length() || m_src[i] == '\n')
            {
                if (currentLine == m_line - 1)
                {
                    line = m_src.substr(lineStart, i - lineStart);
                    break;
                }
                else
                {
                    currentLine++;
                    lineStart = i + 1;
                }
            }
        }

        if (line.empty() && currentLine <= m_line)
        {
            std::cerr << ANSI_ITALIC_ON << "Warning: Requested line number " << m_line << " is out of range." << ANSI_ITALIC_OFF << std::endl;
            return "^EOF -> Invalid line";
        }

        return line;
    }

    inline char consume()
    {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index = 0;
    size_t m_line = 0;
};