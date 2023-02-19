#include <stdio.h>
#include "compiler.h"
#include "helpers/logger.h"

int main(int argc, char *argv[])
{
    if (argc == 1) return 0;

    struct logger* logger = get_logger("main.c");

    char* arg = argv[1];
    int result = compile_file(arg, NULL, 0);
    
    if (result == COMPILER_FILE_COMPILED_OK)
    {
        logger->info(logger, "Everything compiled fine\n");
    }
    else if (result == COMPILER_FILE_FAILED_WITH_ERRORS)
    {
        logger->error(logger, "Compile failed\n");
    }
    else
    {
        logger->error(logger, "Unknown response code %d\n", result);
    }

    kill_all_logger();
    return 0;
}