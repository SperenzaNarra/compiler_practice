#include "parser.h"
#include "parser/parse_helper.h"

#include <stdarg.h>

#include "compiler.h"
#include "node.h"

#include "helpers/vector.h"
#include "helpers/logger.h"

void parse_error(struct logger* logger, struct pos pos, const char* msg, ...)
{
    char buffer[128];
    logger->error(logger, "parse error\n");
    va_list args;
    va_start(args, msg);
    vsprintf(buffer, msg, args);
    logger->error(logger, "%s", buffer);
    va_end(args);

    logger->error(logger, "on line %d col %d in file %s\n", pos.line, pos.col, pos.filename);
    exit(-1);
}

int parse_next()
{
    return 0;
}

int parse(struct compile_process* process)
{
    process->node_vec = vector_create(sizeof(struct node));
    if (!process->node_vec) return PARSE_GENERAL_ERROR;

    process->node_tree_vec = vector_create(sizeof(struct node));
    if (!process->node_tree_vec) return PARSE_GENERAL_ERROR;

    parse_compile_process = process;

    vector_set_peek_pointer(process->token_vec, 0);

    struct node node;
    while (parse_next() == 0)
    {
        // node = node_peek();
        vector_push(process->node_tree_vec, &node);
    }

    return PARSE_ALL_OK;
}