#include <optional>
#include <string>

#ifndef Token_H
#define Token_H


enum class TokenType {
    HEADER,
    _exit,
    ident,
    int_lit,
    semi,
    open_bracket,
    close_bracket,
    _int,
    assign,
    _func,
    _while,
    _if,
    _else,
    _char
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

#endif // Token_H