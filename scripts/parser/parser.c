#include "parser.h"
#include "parser/parse_helper.h"

#include <stdarg.h>
#include <stdbool.h>

#include "compiler.h"
#include "token.h"
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

static bool parser_ignore_cases(struct token* token)
{
    switch (token->type)
    {
    case TOKEN_TYPE_SYMBOL:
        return token->cval == '\\';
    case TOKEN_TYPE_COMMENT:
    case TOKEN_TYPE_NEWLINE:
        return true;
    default:
        return false;
    }
}

static void parser_ignore_handler(struct token* token)
{
    while (token && !parser_ignore_cases(token))
    {
        // skip token
        vector_peek(parser_current_process->token_vec);
        token = vector_peek_no_increment(parser_current_process->token_vec);
    }
}

static struct token* token_peek_next()
{
    struct logger* logger = get_logger("parser.c", "token_peek_next");
    struct token* next_token = vector_peek_no_increment(parser_current_process->token_vec);
    parser_ignore_handler(next_token);
    return vector_peek_no_increment(parser_current_process->token_vec);
}

static struct token* token_next()
{
    struct logger* logger = get_logger("parser.c", "token_next");
    struct token* next_token = vector_peek_no_increment(parser_current_process->token_vec);
    parser_ignore_handler(next_token);
    parser_current_process->pos = next_token->pos;
    parser_last_token = next_token;
    return vector_peek(parser_current_process->token_vec);
}

void parse_single_token_to_node()
{
    struct logger* logger = get_logger("parser.c", "parse_single_token_to_node");

    struct token* token = token_next();
    struct node* node;
    
    switch (token->type)
    {
        case TOKEN_TYPE_NUMBER:
            node = node_create(&(struct node){
                .type = NODE_TYPE_NUMBER,
                .pos = token->pos,
                .llnum = token->llnum
            });
            break;

        case TOKEN_TYPE_IDENTIFIER:
            node = node_create(&(struct node){
                .type = NODE_TYPE_IDENTIFIER,
                .pos = token->pos,
                .sval = token->sval
            });
            break;

        case TOKEN_TYPE_STRING:
            node = node_create(&(struct node){
                .type = NODE_TYPE_STRING,
                .pos = token->pos,
                .sval = token->sval
            });
            break;

        default:
            parse_error(logger, token->pos, "This is not a single token that can be converted to a node");
            break;
    }
}

int parse_next()
{
    struct logger* logger = get_logger("parser.c", "parse_next");

    struct token* token = token_peek_next();
    if (!token) return -1;
    read_token(token);

    int res;
    switch (token->type)
    {
    case TOKEN_TYPE_NUMBER:
    case TOKEN_TYPE_IDENTIFIER:
    case TOKEN_TYPE_STRING:
        parse_single_token_to_node();
        break;
    
    default:
        res = 0;
        break;
    }

    return res;
}

int parse(struct compile_process* process)
{
    struct logger* logger = get_logger("parser.c", "parse");

    process->node_vec = vector_create(sizeof(struct node));
    if (!process->node_vec) return PARSE_GENERAL_ERROR;

    process->node_tree_vec = vector_create(sizeof(struct node));
    if (!process->node_tree_vec) return PARSE_GENERAL_ERROR;

    node_set_vector(process->node_vec, process->node_tree_vec);
    parser_current_process = process;

    vector_set_peek_pointer(process->token_vec, 0);

    struct node node;
    while (parse_next() == 0)
    {
        // node = node_peek();
        vector_push(process->node_tree_vec, &node);
    }

    return PARSE_ALL_OK;
}