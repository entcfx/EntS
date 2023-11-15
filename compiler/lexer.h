#include <stdint.h>
#include <stddef.h>

#ifndef TOKENS_H
#define TOKENS_H

#define KEYWORD 1
#define IDENTIFIER 2
#define OPERATOR 3
#define PLUS 4
#define MINUS 5
#define MULTIPLY 6
#define DIVIDE 7
#define INTEGER_LITERAL 8
#define FLOAT_LITERAL 9
#define CHAR_LITERAL 10
#define STRING_LITERAL 11
#define COMMENT 12
#define LEFT_PAREN 13
#define RIGHT_PAREN 14
#define LEFT_BRACKET 15
#define RIGHT_BRACKET 16
#define COMMA 17
#define ASSIGN 18
#define ADD_ASSIGN 19
#define SUB_ASSIGN 20
#define EQUAL 21
#define NOT_EQUAL 22
#define LESS_THAN 23
#define GREATER_THAN 24
#define AND 25
#define OR 26
#define BITAND 27
#define BITOR 28

#define SEMICOLON 100

typedef struct {
    int type;        // Token type (e.g., keyword, identifier, etc.)
    char* lexeme;    // Token lexeme (actual text)
    int line;
} Token;

// Define a structure to represent a list of tokens
typedef struct {
    Token* tokens;   // Array of tokens
    size_t count;    // Number of tokens in the list
} TokenList;

#endif // TOKENS_H

TokenList* lex(const char* sourceCode);

void freeTokenList(TokenList* tokenList);
void addToken(TokenList* tokenList, int type, const char* lexeme, int line);