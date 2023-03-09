#include "parser.h"
#include "parser/parse_helper.h"

#include "compiler.h"
#include "node.h"

#include "helpers/vector.h"

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