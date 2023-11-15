#include "func.h"
#include "lexer.h"
#include <stdlib.h>

char input_file[MAX_FILENAME_LENGTH];
char output_file[MAX_FILENAME_LENGTH];

int main(int argc, char **argv) {
    // Prevent a SEG fault
    if (argc < 3) {
        printErrorMessage("Not enough arguments.");
        fprintf(stderr, "Usage: \033[3m<inputfile>\033[0m \033[3m<output>\033[0m\n");
        return -1;
    }

    // Check for compiler flags
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            // This is a flag
            if (!isValidFlag(argv[i])) {
                printErrorMessage("Invalid compiler flag.");
                fprintf(stderr, "Usage: %s", argv[0]);
                for (int j = 0; compilerFlags[j].flag != NULL; ++j) {
                    fprintf(stderr, " [\033[1;37;1m%s\033[0m]", compilerFlags[j].flag);
                }
                fprintf(stderr, " \033[3m<inputfile>\033[0m \033[3m<output>\033[0m\n");
                return -1;
            }

            for (int j = 0; compilerFlags[j].flag != NULL; ++j) {
                if (strcmp(compilerFlags[j].flag, argv[i]) == 0) {
                    compilerFlags[j].enabled = true;
                    break;
                }
            }
        }
    }

    // Find input and output files
    int fileCount = 0;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            // This is not a flag, it's a file argument
            if (fileCount == 0) {
                strncpy(input_file, argv[i], MAX_FILENAME_LENGTH - 1);
                input_file[MAX_FILENAME_LENGTH - 1] = '\0';
                fileCount++;
            } else if (fileCount == 1) {
                strncpy(output_file, argv[i], MAX_FILENAME_LENGTH - 1);
                output_file[MAX_FILENAME_LENGTH - 1] = '\0';
                fileCount++;
            } else {
                // Already found two files, any additional non-flag argument is an error
                printErrorMessage("Invalid usage. Too many file arguments.");
                fprintf(stderr, "Usage: %s", argv[0]);
                for (int j = 0; compilerFlags[j].flag != NULL; ++j) {
                    fprintf(stderr, " [\033[1;37;1m%s\033[0m]", compilerFlags[j].flag);
                }
                fprintf(stderr, " \033[3m<inputfile>\033[0m \033[3m<output>\033[0m\n");
                return -1;
            }
        }
    }

    // Check if we found both input and output files
    if (input_file[0] == '\0' || output_file[0] == '\0') {
        printErrorMessage("Invalid usage. Missing input or output file.");
        fprintf(stderr, "Usage: %s", argv[0]);
        for (int i = 0; compilerFlags[i].flag != NULL; ++i) {
            fprintf(stderr, " [\033[1;37;1m%s\033[0m]", compilerFlags[i].flag);
        }
        fprintf(stderr, " \033[3m<inputfile>\033[0m \033[3m<output>\033[0m\n");
        return -1;
    }

    // Actually compiling
    // Load the input file from the disk
    
    FILE *inputFile = fopen(input_file, "r");
    if (inputFile == NULL) {
        // Print the error and info into stderr
        char errMsgBuffer[256];
        sprintf(errMsgBuffer, "Could not open input file \"%s\"!", input_file);
        printErrorMessage(errMsgBuffer);
    }
    // Read all of the contents of the file to memory
    fseek(inputFile, 0, SEEK_END);
    size_t fileSize = ftell(inputFile);
    rewind(inputFile);
    char *sourceCode = (char *)malloc(fileSize + 1);

    // Get a handler to the output file
    FILE *outputFile = fopen(output_file, "w");
    if (outputFile != NULL) {
        // Print the error and info into stderr
        char errMsgBuffer[256];
        sprintf(errMsgBuffer, "Output file \"%s\" already exists!", output_file);
        printErrorMessage(errMsgBuffer);
    }

    if (sourceCode == NULL) {
        // Handle memory allocation failure
        printErrorMessage("Memory allocation error!");
    } else {
        size_t bytesRead = fread(sourceCode, 1, fileSize, inputFile);

        if (bytesRead != fileSize) {
            // Handle read error
            printErrorMessage("Error reading file contents!");
            free(sourceCode); // Free allocated memory
        } else {
            sourceCode[fileSize] = '\0';
            // Lexer here
            TokenList* tokens = lex(sourceCode);
            TokenList tokenList = *tokens;
            // DEBUG
            for (size_t i = 0; i < tokenList.count; ++i) {
                printf("Token %zu:\n", i + 1);
                printf("Type: %d\n", tokenList.tokens[i].type);
                printf("Lexeme: %s\n", tokenList.tokens[i].lexeme);
                printf("\n");
            }
            char *asembly = generateAsm(tokenList, output_file);
            free(sourceCode);
        }
    }
    fclose(inputFile);
    return 0;
}
