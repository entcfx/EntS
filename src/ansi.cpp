#include <iostream>
#include "ansi.h"
#include <cstdarg>

[[noreturn]] void errorf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR;
    vfprintf(stderr, format, args);
    std::cerr << std::endl;

    va_end(args);

    std::cerr << "compilation terminated." << std::endl;
    exit(EXIT_FAILURE);
}