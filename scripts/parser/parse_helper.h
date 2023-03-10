#ifndef PARSE_HELPER_H
#define PARSE_HELPER_H

#include "compiler.h"
#include "node.h"

#include "helpers/logger.h"

struct compile_process* parse_compile_process;

void parse_error(struct logger* logger, struct pos pos, const char* msg, ...);

#endif // PARSE_HELPER_H