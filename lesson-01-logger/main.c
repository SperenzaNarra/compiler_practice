#include <stdio.h>
#include "helpers/logger.h"

int main(int argc, char *argv[])
{
    struct logger* logger = get_logger("main.c");
    logger->info(logger, "%s test\n", "info");
    logger->debug(logger, "%s test\n", "debug");
    logger->warning(logger, "%s test\n", "warning");
    logger->error(logger, "%s test\n", "error");
    kill_all_logger();

    if (argc == 1) return 0;

    char* arg = argv[1];
    printf("%s\n", arg);
    return 0;
}