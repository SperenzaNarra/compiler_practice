#ifndef PARSE_HELPER_H
#define PARSE_HELPER_H

#include "compiler.h"
#include "token.h"
#include "node.h"
#include "history.h"

#include "helpers/logger.h"

struct compile_process* parser_current_process;
struct token* parser_last_token;

void parse_error(struct pos pos, const char* msg, ...);
struct token* token_peek_next();
struct token* token_next();
void parse_single_token_to_node();
int parse_next();

// parser_exp
void parse_expressionable(struct history* history);


#endif // PARSE_HELPER_H