#ifndef PARSER_H
#define PARSER_H

#include "compiler.h"

enum
{
    PARSE_ALL_OK,
    PARSE_GENERAL_ERROR,
};

// parser.c
int parse(struct compile_process* process);

#endif