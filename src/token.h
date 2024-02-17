#pragma once
#include <string>
#include <optional>
#include <iostream>
#include <unordered_map>

#ifndef Token_H
#define Token_H

enum class TokenType
{
    TkEof = -1,
    NotEqual,         // !=
    Not,              // !
    ModEqual,         // %=
    Modular,          // %
    BitAnd,           // &&
    AndEqual,         // &=
    And,              // &
    OpenBracket,      // (
    CloseBracket,     // )
    StarEqual,        // *=
    Star,             // *
    Increment,        // ++
    PlusEqual,        // +=
    Plus,             // +
    Comma,            // ,
    Decrement,        // --
    MinusEqual,       // -=
    Arrow,            // ->
    Minus,            // -
    Dot,              // .
    DivEqual,         // /=
    Div,              // /
    Colon,            // :
    Semicolon,        // ;
    ShiftLeftAssign,  // <<=
    ShiftLeft,        // <<
    LessEqual,        // <=
    Less,             // <
    Equal,            // ==
    Assign,           // =
    ShiftRightAssign, // >>=
    ShiftRight,       // >>
    GreaterEqual,     // >=
    Greater,          // >
    Question,         // ?
    OpenSquare,       // [
    CloseSquare,      // ]
    XorEqual,         // ^=
    Xor,              // ^
    OpenCurly,        // {
    CloseCurly,       // }
    OrEqual,          // |=
    Or,               // |
    BitOr,            // ||
    Tilde,            // ~

    uint8,
    uint16,
    uint32,
    uint64,
    int8,
    int16,
    int32,
    int64,
    Char, // uint8 but a char uk

    Break,
    Case,
    Continue,
    Default,
    Else,
    Extern,
    For,
    If,
    Int,
    Header,
    Function,
    Return,
    Typedef,
    Struct,
    Switch,
    Sizeof,
    Void,
    While,

    _Hex,
    _Decimal,
    _Binary,
    _Identifier,
    _String,
    _CharLit,
    _DecimFloat,
};

typedef struct
{
    TokenType type;
    std::optional<std::string> value{};
} Token;

std::string token_type_to_string(TokenType type);

std::string token_to_string(Token token);
#endif