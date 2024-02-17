#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include "lexer.h"
#include "token.h"
#include "ansi.h"
#include "ast.h"
#include "parser.h"

void printAST(const std::unique_ptr<Node>& node, int depth) {
    std::cout << std::string(depth, ' ') << node->type;
    if (node->leaf) {
        std::cout << " " << *node->leaf;
    }
    std::cout << std::endl;
    for (const auto& child : node->children) {
        printAST(child, depth + 1);
    }
}

int main(int argc, char* argv[]) {
    // Variables
    std::string sourceFile;
    
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            // It's a flag
        } else {
            // It's the source file
            if (sourceFile.empty()) {
                sourceFile = argv[i];
            } else {
                std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR
                        << "multiple source files not allowed" << std::endl;
                std::cerr << "compilation terminated." << std::endl;
                return EXIT_FAILURE;
            }
        }
    }

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

    // Tokenization
    Tokenizer tokenizer = Tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();

    // Make the AST
    Parser parser = Parser(tokens);
    std::unique_ptr<Node> ast = parser.parse();

    // Print AST
    printAST(ast, 0);


#ifdef DEBUG
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
        
        // WRITTE ASSEMBLY TO outFile:
        // outFile << 

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
        // WRITTE ASSEMBLY TO count:
        // count << 
    }
#endif

    input.close();
    return EXIT_SUCCESS;
}