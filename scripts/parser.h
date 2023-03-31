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

// parser_exp.c
struct node* make_exp_node(struct node* left, struct node* right, const char* op);

#endif