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

#endif // PARSE_HELPER_H