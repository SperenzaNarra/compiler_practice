#include <stdio.h>
#include "compiler.h"
#include "helpers/logger.h"

int main(int argc, char *argv[])
{
    if (argc == 1) return 0;

    char* arg = argv[1];
    int result = compile_file(arg, NULL, 0);
    
    if (result == COMPILER_FILE_COMPILED_OK)
    {
        log_info("Everything compiled fine\n");
    }
    else if (result == COMPILER_FILE_FAILED_WITH_ERRORS)
    {
        log_error("Compile failed\n");
    }
    else
    {
        log_error("Unknown response code %d\n", result);
    }
    return 0;
}