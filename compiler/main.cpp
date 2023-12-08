#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include "lexer.h"
#include "token.h"
#include "asm.h"
#include "ansi.h"
#include "parser.h"


int main(int argc, char* argv[]) {

    // Arguments check
    if (argc < 2) 
    {
        std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR
                  << "no input files" << std::endl;
        std::cerr << "compilation terminated." << std::endl;
        return EXIT_FAILURE;
    }

    // Reading the file into a stream
    std::fstream input(argv[1], std::ios::in);

    // File is open check
    if (!input.is_open()) 
    {
        std::cerr   << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR
                    << "file not found: \"" << argv[1]
                    << "\"." << std::endl;
        return EXIT_FAILURE;
    }

    // Getting the string
    std::string contents;
    {
        std::stringstream contents_stream;
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }

    Tokenizer tokenizer_obj(std::move(contents));
    std::vector<Token> tokens = tokenizer_obj.tokenize();

    // Parser
    Parser parser(std::move(tokens));
    std::optional<NodeProg> prog = parser.parse_prog();

    if (!prog.has_value()) {
        std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED
        << "fatal error: " << ANSI_RESET_COLOR << "parser error " << std::endl;
        exit(EXIT_FAILURE);
    }

    // Generator
    Generator generator(prog.value());
    
    auto outputFlagPos = std::find(argv, argv + argc, std::string("-o"));

    if (outputFlagPos != argv + argc && outputFlagPos + 1 != argv + argc) {
        // Found -o flag and there is a path following it
        std::string outputPath = outputFlagPos[1];
        std::string asmPath = outputPath + ".asm";
        std::string objPath = outputPath + ".o";
        // Open a fstream to the path and validate it is open
        std::ofstream outFile(asmPath);
        if(!outFile.good()){
            std::cerr   << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR 
                        << "failed to create or write to file at \"" << ANSI_ITALIC_ON << outputPath << ANSI_ITALIC_OFF
                        << "\"" << std::endl;
                        std::cerr << "compilation terminated." << std::endl;
            return EXIT_FAILURE;
        }
        // Writte the result of tokens_to_asm(tokens); to the file
        outFile << generator.gen_prog();
        outFile.close();

        std::string nasmCommand = "nasm -felf64 " + asmPath;
        if (std::system(nasmCommand.c_str()) != 0) {
            std::cerr << "NASM assembly failed." << std::endl;
            std::cerr << "compilation terminated." << std::endl;
            return EXIT_FAILURE;
        }

        // Link with LD
        std::string ldCommand = "ld -o " + outputPath + " " + objPath;
        if (std::system(ldCommand.c_str()) != 0) {
            std::cerr << "Linking failed." << std::endl;
            std::cerr << "compilation terminated." << std::endl;
            return EXIT_FAILURE;
        }

        // Remove intermediate files
        std::remove(asmPath.c_str());
        std::remove(objPath.c_str());

    } else {
        std::cout << generator.gen_prog();
    }
    

    input.close();
    return EXIT_SUCCESS;
}
