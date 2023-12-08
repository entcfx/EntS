#ifndef ANSI_H
#define ANSI_H

// ANSI Escape
const char ANSI_RESET_COLOR[] = "\033[0m";
const char ANSI_BOLD_WHITE[] = "\033[1;37m";
const char ANSI_BOLD_RED[] = "\033[1;91;1m";
const char ANSI_BOLD_GREEN[] = "\033[1;92m";
const char ANSI_BOLD_BLUE[] = "\033[1;94m";
const char ANSI_BOLD_YELLOW[] = "\033[1;93m";

// Non-bold colors
const char ANSI_WHITE[] = "\033[0;37m";
const char ANSI_RED[] = "\033[0;91m";
const char ANSI_GREEN[] = "\033[0;92m";
const char ANSI_BLUE[] = "\033[0;94m";
const char ANSI_YELLOW[] = "\033[0;93m";

// Italic
const char ANSI_ITALIC_ON[] = "\033[3m";
const char ANSI_ITALIC_OFF[] = "\033[23m";

#endif // ANSI_H
