#pragma once
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FLAGS 10
#define MAX_FILENAME_LENGTH 256

typedef struct {
    const char* flag;
    const char* description;
    bool enabled;
} CompilerFlag;

extern CompilerFlag compilerFlags[];

bool isFlagPresent(const char* flag, int argc, char** argv);
bool isValidFlag(const char* flag);
void printErrorMessage(const char* message);