#ifndef PARSER_H
#define PARSER_H

#include "compiler.h"

enum
{
    PARSE_ALL_OK,
    PARSE_GENERAL_ERROR,
};

int parse(struct compile_process* process);

#endif