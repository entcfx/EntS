#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"

void *generateAsm(TokenList tokenList, FILE outputFile);
void getNextToken(TokenList* tokenList);