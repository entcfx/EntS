#include "token.h"

std::string token_type_to_string(TokenType type)
{
    static const std::unordered_map<TokenType, std::string> tokenTypeStrings = {
        {TokenType::TkEof, "TkEof"},
        {TokenType::NotEqual, "NotEqual"},
        {TokenType::Not, "Not"},
        {TokenType::ModEqual, "ModEqual"},
        {TokenType::Modular, "Modular"},
        {TokenType::BitAnd, "BitAnd"},
        {TokenType::AndEqual, "AndEqual"},
        {TokenType::And, "And"},
        {TokenType::OpenBracket, "OpenBracket"},
        {TokenType::CloseBracket, "CloseBracket"},
        {TokenType::StarEqual, "StarEqual"},
        {TokenType::Star, "Star"},
        {TokenType::Increment, "Increment"},
        {TokenType::PlusEqual, "PlusEqual"},
        {TokenType::Plus, "Plus"},
        {TokenType::Comma, "Comma"},
        {TokenType::Decrement, "Decrement"},
        {TokenType::MinusEqual, "MinusEqual"},
        {TokenType::Arrow, "Arrow"},
        {TokenType::Minus, "Minus"},
        {TokenType::Dot, "Dot"},
        {TokenType::DivEqual, "DivEqual"},
        {TokenType::Div, "Div"},
        {TokenType::Colon, "Colon"},
        {TokenType::Semicolon, "Semicolon"},
        {TokenType::ShiftLeftAssign, "ShiftLeftAssign"},
        {TokenType::ShiftLeft, "ShiftLeft"},
        {TokenType::LessEqual, "LessEqual"},
        {TokenType::Less, "Less"},
        {TokenType::Equal, "Equal"},
        {TokenType::Assign, "Assign"},
        {TokenType::ShiftRightAssign, "ShiftRightAssign"},
        {TokenType::ShiftRight, "ShiftRight"},
        {TokenType::GreaterEqual, "GreaterEqual"},
        {TokenType::Greater, "Greater"},
        {TokenType::Question, "Question"},
        {TokenType::OpenSquare, "OpenSquare"},
        {TokenType::CloseSquare, "CloseSquare"},
        {TokenType::XorEqual, "XorEqual"},
        {TokenType::Xor, "Xor"},
        {TokenType::OpenCurly, "OpenCurly"},
        {TokenType::CloseCurly, "CloseCurly"},
        {TokenType::OrEqual, "OrEqual"},
        {TokenType::Or, "Or"},
        {TokenType::BitOr, "BitOr"},
        {TokenType::Tilde, "Tilde"},
        {TokenType::uint8, "uint8"},
        {TokenType::uint16, "uint16"},
        {TokenType::uint32, "uint32"},
        {TokenType::uint64, "uint64"},
        {TokenType::int8, "int8"},
        {TokenType::int16, "int16"},
        {TokenType::int32, "int32"},
        {TokenType::int64, "int64"},
        {TokenType::Char, "Char"},
        {TokenType::Break, "Break"},
        {TokenType::Case, "Case"},
        {TokenType::Continue, "Continue"},
        {TokenType::Default, "Default"},
        {TokenType::Else, "Else"},
        {TokenType::Extern, "Extern"},
        {TokenType::For, "For"},
        {TokenType::If, "If"},
        {TokenType::Int, "Int"},
        {TokenType::Header, "Header"},
        {TokenType::Function, "Function"},
        {TokenType::Return, "Return"},
        {TokenType::Typedef, "Typedef"},
        {TokenType::Struct, "Struct"},
        {TokenType::Switch, "Switch"},
        {TokenType::Sizeof, "Sizeof"},
        {TokenType::Void, "Void"},
        {TokenType::While, "While"},
        {TokenType::_Hex, "_Hex"},
        {TokenType::_Decimal, "_Decimal"},
        {TokenType::_Binary, "_Binary"},
        {TokenType::_Identifier, "_Identifier"},
        {TokenType::_String, "_String"},
        {TokenType::_CharLit, "_CharLit"},
        {TokenType::_DecimFloat, "_DecimFloat"},
    };

    auto it = tokenTypeStrings.find(type);
    if (it != tokenTypeStrings.end())
    {
        return it->second;
    }
    else
    {
        return "Unknown";
    }
}

std::string token_to_string(Token token)
{
    std::string result = "Token Name: " + token_type_to_string(token.type) + " - {";

    if (token.value)
    {
        result += token_type_to_string(token.type) + " : " + *token.value;
    }
    else
    {
        result += token_type_to_string(token.type) + " : 'none'";
    }

    result += "}";
    return result;
}
