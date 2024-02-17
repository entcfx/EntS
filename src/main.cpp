#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <utility>
#include <cstdlib>
#include <algorithm>

#include "ast.h"
#include "ansi.h"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char *argv[])
{
    std::string sourceFile;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            // It's a flag
        }
        else
        {
            // It's the source file
            if (sourceFile.empty())
            {
                sourceFile = argv[i];
            }
            else
            {
                errorf("multiple source files not allowed");
            }
        }
    }

    if (sourceFile.empty())
    {
        errorf("no input files");
    }

    std::fstream input(argv[1], std::ios::in);
    if (!input.is_open())
    {
        errorf("file not found: \"%s\".", argv[1]);
    }

    std::string contents;
    {
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();

        contents = contents_stream.str();
    }
    input.close();

    Tokenizer tokenizer = Tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();

    // Print the tokens
    for (const auto &t : tokens)
    {
        std::cout << token_to_string(t) << "\n";
    }

    return EXIT_SUCCESS;
}
