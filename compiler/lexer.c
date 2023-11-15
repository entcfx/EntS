#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

TokenList* lex(const char* sourceCode) {
    TokenList* tokenList = (TokenList*)malloc(sizeof(TokenList));
    tokenList->tokens = NULL;
    tokenList->count = 0;
    int line = 1;

    const char* keywords[] = {"int", "uint", "float", "double", "char", "bool", "NULL", "int8_t", "uint8_t", "int16_t", "uint16_t", "main", "return", "void", "exit", 
    "if", "else", "while", "switch", "case", "break", "continue", "true", "false", "call", "typedef", "extern", "default", "typeof"};
    const char* operators[] = {"+", "-", "*", "/", "&", "|"};
    const char* punctuation[] = {"(", ")", "{", "}", ",", ";", "[", "]", "=", "+=", "-=", "==", "!=", "<", ">", "&&", "||"};
    if (*sourceCode == '\n') {
        line++;
    }
    while (*sourceCode != '\0') {
        // Skip whitespace
        while (isspace(*sourceCode)) {
            sourceCode++;
        }
        // Check for comments
        if (strncmp(sourceCode, "//", 2) == 0) {
            // Skip the entire comment line
            while (*sourceCode != '\0' && *sourceCode != '\n') {
                sourceCode++;
            }
            // Continue to the next line or the end of the file
            continue;
        }
        // Check for keywords
        for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) {
            size_t len = strlen(keywords[i]);
            if (strncmp(sourceCode, keywords[i], len) == 0 && !isalnum(sourceCode[len])) {
                addToken(tokenList, KEYWORD, keywords[i], line);
                sourceCode += len;
                break;
            }
        }
        // Check for identifiers
        if (isalpha(*sourceCode)) {
            const char* start = sourceCode;
            while (isalnum(*sourceCode) || *sourceCode == '_') {
                sourceCode++;
            }
            size_t len = sourceCode - start;
            addToken(tokenList, IDENTIFIER, strndup(start, len), line);
        }
        // Check for operators
        for (size_t i = 0; i < sizeof(operators) / sizeof(operators[0]); ++i) {
            size_t len = strlen(operators[i]);
            if (strncmp(sourceCode, operators[i], len) == 0) {
                // Add token for the operator
                switch (i) {
                    case 0: addToken(tokenList, PLUS, operators[i], line); break;
                    case 1: addToken(tokenList, MINUS, operators[i], line); break;
                    case 2: addToken(tokenList, MULTIPLY, operators[i], line); break;
                    case 3: addToken(tokenList, DIVIDE, operators[i], line); break;
                    case 4: addToken(tokenList, BITAND, operators[i], line); break;
                    case 5: addToken(tokenList, BITOR, operators[i], line); break;
                    // Add more cases for other operators if needed
                }
                sourceCode += len;
                break;
            }
        }
        // Check for literals
        if (isDigit(*sourceCode)) {
            // Handle integer or floating-point literals
            const char* start = sourceCode;
            while (isDigit(*sourceCode) || *sourceCode == '.') {
                sourceCode++;
            }
            size_t len = sourceCode - start;
            addToken(tokenList, isdigit(start[len - 1]) ? INTEGER_LITERAL : FLOAT_LITERAL, strndup(start, len), line);
        }
        // Check for character literals
        if (*sourceCode == '\'') {
            sourceCode++;  // Consume the opening single quote
            if (*sourceCode == '\\') {
                // Handle escape sequences if present
                sourceCode++;
            }
            if (*sourceCode == '\0' || *(sourceCode + 1) != '\'') {
                // Handle error: character literal must end with a single quote
                // Add appropriate error handling or simply ignore the invalid character literal
            } else {
                // Add token for character literal
                addToken(tokenList, CHAR_LITERAL, strndup(sourceCode, 2), line);
                sourceCode += 2;  // Consume the character and the closing single quote
            }
        }
        // Check for string literals
        if (*sourceCode == '"') {
            sourceCode++;  // Consume the opening double quote
            const char* start = sourceCode;
            while (*sourceCode != '\0' && *sourceCode != '"') {
                if (*sourceCode == '\\') {
                    // Handle escape sequences if present
                    sourceCode++;
                }
                sourceCode++;
            }
            if (*sourceCode == '\0') {
                // Handle error: unterminated string literal
                // Add appropriate error handling or simply ignore the unterminated string literal
            } else {
                // Add token for string literal
                addToken(tokenList, STRING_LITERAL, strndup(start, sourceCode - start), line);
                sourceCode++;  // Consume the closing double quote
            }
        }
        // Check for punctuation
        for (size_t i = 0; i < sizeof(punctuation) / sizeof(punctuation[0]); ++i) {
            size_t len = strlen(punctuation[i]);
            if (strncmp(sourceCode, punctuation[i], len) == 0) {
                // Add token for the punctuation
                switch (i) {
                    case 0: addToken(tokenList, LEFT_PAREN, punctuation[i], line); break;
                    case 1: addToken(tokenList, RIGHT_PAREN, punctuation[i], line); break;
                    case 2: addToken(tokenList, LEFT_BRACKET, punctuation[i], line); break;
                    case 3: addToken(tokenList, RIGHT_BRACKET, punctuation[i], line); break;
                    case 4: addToken(tokenList, COMMA, punctuation[i], line); break;
                    case 5: addToken(tokenList, SEMICOLON, punctuation[i], line); break;
                    case 6: addToken(tokenList, ASSIGN, punctuation[i], line); break;
                    case 7: addToken(tokenList, ADD_ASSIGN, punctuation[i], line); break;
                    case 8: addToken(tokenList, SUB_ASSIGN, punctuation[i], line); break;
                    case 9: addToken(tokenList, EQUAL, punctuation[i], line); break;
                    case 10: addToken(tokenList, NOT_EQUAL, punctuation[i], line); break;
                    case 11: addToken(tokenList, LESS_THAN, punctuation[i], line); break;
                    case 12: addToken(tokenList, GREATER_THAN, punctuation[i], line); break;
                    case 13: addToken(tokenList, AND, punctuation[i], line); break;
                    case 14: addToken(tokenList, OR, punctuation[i], line); break;
                    // Add more cases for other punctuation if needed
                }
                sourceCode += len;
                break;
            }
        }
    }
    return tokenList;
}

void freeTokenList(TokenList* tokenList) {
    if (tokenList != NULL) {
        for (size_t i = 0; i < tokenList->count; ++i) {
            free(tokenList->tokens[i].lexeme);
        }
        free(tokenList->tokens);
        free(tokenList);
    }
}
void addToken(TokenList* tokenList, int type, const char* lexeme, int line) {
    tokenList->tokens = realloc(tokenList->tokens, (tokenList->count + 1) * sizeof(Token));
    tokenList->tokens[tokenList->count].type = type;
    tokenList->tokens[tokenList->count].lexeme = strdup(lexeme);
    tokenList->tokens[tokenList->count].line = line;
    tokenList->count++;
}
