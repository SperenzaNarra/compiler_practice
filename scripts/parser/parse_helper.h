#ifndef PARSE_HELPER_H
#define PARSE_HELPER_H

#include "compiler.h"
#include "token.h"
#include "node.h"
#include "history.h"

#include "helpers/logger.h"

struct compile_process* current_compiler_process;
struct token* parser_last_token;

// parser.c
void parse_single_token_to_node();
struct token* parse_peek_token();
struct token* parse_next_token();

// parser_exp.c
void parse_expressionable(struct history* history);

#endif // PARSE_HELPER_H