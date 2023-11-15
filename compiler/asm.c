#include "asm.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

void getNextToken(TokenList* tokenList) {
    // Check if there are more tokens
    if (tokenList->count > 0) {
        // Move to the next token by incrementing the pointer and decrementing the count
        tokenList->tokens++;
        tokenList->count--;
    }
}

void *generateAsm(TokenList tokenList, FILE outputFile) {
    // Basic assembly setup
    printf("bits 64\n");
    printf("global _start\n");
    printf("section .text\n");

    TokenList* tokenListPtr = &tokenList;

    while (tokenList.count > 0) {
        // Check for a identifier
        if (tokenList.tokens[0].type == IDENTIFIER) {
            char* functionName = tokenList.tokens[0].lexeme;

            // Move to the next token
            getNextToken(tokenListPtr);

            // Check if it is followed by a left parenthesis
            if (tokenList.tokens[0].type == LEFT_PAREN) {
                // Move to the next token
                getNextToken(tokenListPtr);

                // Check for parameter list valid are: KEYWORD, IDENTIFIER, COMMA, END
                // Anything else must be a user error
                if (tokenList.tokens[0].type == KEYWORD || tokenList.tokens[0].type == IDENTIFIER || tokenList.tokens[0].type == COMMA || tokenList.tokens[0].type == RIGHT_PAREN) {
                    // Move to the next token
                    getNextToken(tokenListPtr);

                    // Check if it is followed by a left curly brace
                    if (tokenList.tokens[0].type == LEFT_BRACKET) {
                        // Create a label for the function in the assembly code
                        printf("%s:\n", functionName);

                        //TODO Actually make the assembly label

                        // Move to the next token after the function declaration
                        getNextToken(tokenListPtr);
                    } else {
                        // Throw an error: Expected LEFT_BRACKET after parameter list
                        // Handle error or exit
                        // Get the lexeme of the token that caused the error
                        char* errLexeme = tokenList.tokens[0].lexeme;
                        // Get the line of the errorus token
                        int errLineNum = tokenList.tokens[0].line;
                        // Print the lexeme and its type
                        fprintf(stderr, "Error on line %d: Expected '(' at '%s'\n", errLineNum, errLexeme);
                        exit(-1);
                    }
                } else {
                    // Get the lexeme of the token that caused the error
                    char* errLexeme = tokenList.tokens[0].lexeme;
                    // Get the line of the errorus token
                    int errLineNum = tokenList.tokens[0].line;
                    // Print the lexeme and its type
                    fprintf(stderr, "Error on line %d: Unexpected identifier '%s'\n", errLineNum, errLexeme);
                    exit(-1);
                }
            } else {
                // Identifier is NOT a function just exit
            }
        }
        //{"int", "uint", "float", "double", "char", "bool", "NULL", "int8_t", "uint8_t", "int16_t", "uint16_t", "main", "return", "void", "exit", 
        // "if", "else", "while", "switch", "case", "break", "continue", "true", "false", "call", "typedef", "extern", "default", "typeof"};
        if (tokenList.tokens[0].type == KEYWORD) {
            // The token is a keyword, and as it we have to handle it.
            if (tokenList.tokens[0].lexeme == "call") {
                // Call found, load brackets
                getNextToken(tokenListPtr);
                if (tokenList.tokens[0].type == LEFT_PAREN) {
                    // Read arguemnts
                } else {

                }
            }
        }
    }
}