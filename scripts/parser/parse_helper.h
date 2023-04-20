#ifndef PARSE_HELPER_H
#define PARSE_HELPER_H

#include <string.h>

#include "compiler.h"
#include "token.h"
#include "node.h"
#include "history.h"

#include "helpers/logger.h"

#define TOTAL_OPERATOR_GROUPS 14
#define MAX_OPERATOR_IN_GROUP 12

#define S_EQ(str1, str2) (str1 && str2 && (strcmp(str1, str2) == 0))

enum
{
    ASSOCIATIVITY_LEFT_TO_RIGHT,
    ASSOCIATIVITY_RIGHT_TO_LEFT,

};

struct expressionable_op_precedence_group
{
    char* operators[MAX_OPERATOR_IN_GROUP];
    int associtivity;
};

struct compile_process* current_compiler_process;
struct token* parser_last_token;

// parser.c

void parse_single_token_to_node();
struct token* parse_peek_token();
struct token* parse_next_token();

// parser_exp.c

void parse_expressionable(struct history* history);

#endif // PARSE_HELPER_H