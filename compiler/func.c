#include "func.h"

// Use a sentinel value to mark the end of the flags array
CompilerFlag compilerFlags[] = {
    {"--compile-only", "Only compile without linking", false},
    // Add more flags as need
    {NULL, NULL, false}  // Sentinel value to mark the end
};

bool isFlagPresent(const char* flag, int argc, char** argv) {
    for (int i = 0; argv[i] != NULL; ++i) {
        if (strcmp(flag, argv[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isValidFlag(const char* flag) {
    for (int i = 0; compilerFlags[i].flag != NULL; ++i) {
        if (strcmp(flag, compilerFlags[i].flag) == 0) {
            return true;
        }
    }
    return false;
}

void printErrorMessage(const char* message) {
    fprintf(stderr, "\033[1ment \033[1;31;1mfatal error:\033[0m %s\n", message);
}