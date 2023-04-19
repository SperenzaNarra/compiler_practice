#include "parser.h"
#include "parser/parse_helper.h"

#include <stdbool.h>

#include "token.h"
#include "helpers/vector.h"

void parser_ignore_nl_or_comment(struct token* token)
{
    while (token && token_is_nl_or_comment_or_newline_separator(token))
    {
        // skip token
        vector_peek(current_compiler_process->token_vec);
        token = vector_peek_no_increment(current_compiler_process->token_vec);
    }

}

struct token* parse_peek_token()
{
    log_debug("is called\n");
    struct token* token = vector_peek_no_increment(current_compiler_process->token_vec);
    parser_ignore_nl_or_comment(token);
    return vector_peek_no_increment(current_compiler_process->token_vec);
}

struct token* parse_next_token()
{
    log_debug("is called\n");
    struct token* next_token = vector_peek_no_increment(current_compiler_process->token_vec);
    parser_ignore_nl_or_comment(next_token);
    if (!next_token) return NULL;
    current_compiler_process->pos = next_token->pos;
    parser_last_token = next_token;
    return vector_peek(current_compiler_process->token_vec);
}

void parse_single_token_to_node()
{
    struct token* token = parse_next_token();
    struct node* node;
    switch (token->type)
    {
    case TOKEN_TYPE_NUMBER:
        node = node_create(&(struct node){
            .type = NODE_TYPE_NUMBER,
            .llnum = token->llnum,
            .pos = token->pos,
        });
        break;
    case TOKEN_TYPE_IDENTIFIER:
        node = node_create(&(struct node){
            .type = NODE_TYPE_IDENTIFIER,
            .sval = token->sval,
            .pos = token->pos,
        });
        break;
    case TOKEN_TYPE_STRING:
        node = node_create(&(struct node){
            .type = NODE_TYPE_STRING,
            .sval = token->sval,
            .pos = token->pos,
        });
        break;
    
    default:
        log_error("invalid token type %d\n", token->type);
        exit(-1);
    }
    
}

int parse_next()
{
    log_debug("is called\n");
    struct token* token = parse_peek_token();
    if (!token) return -1;

    log_info("read token %p\n", token);
    read_token(token);

    int res = 0;
    switch (token->type)
    {
    case TOKEN_TYPE_NUMBER:
    case TOKEN_TYPE_IDENTIFIER:
    case TOKEN_TYPE_STRING:
        parse_single_token_to_node();
        break;
    case TOKEN_TYPE_OPERATOR:
        parse_expressionable(history_begin(0));
        break;
    default:
        log_error("invalid token type %d from\n", token->type);
        read_token(token);
        res = -1;
        break;
    }
    log_debug("return value %d\n", res);
    return res;
}

void read_nodes(struct node* node_root)
{
    if (!node_root) return;

    struct node* node = node_root;
    switch (node->type)
    {
    case NODE_TYPE_NUMBER:
        fprintf(stderr, "%lld ", node->llnum);
        break;
    case NODE_TYPE_IDENTIFIER:
    case NODE_TYPE_STRING:
        fprintf(stderr, "%s ", node->sval);
        break;
    case NODE_TYPE_EXPRESSION:
        fprintf(stderr, "( ");
        read_nodes(node->exp.left);
        fprintf(stderr, "%s ", node->exp.op);
        read_nodes(node->exp.right);
        fprintf(stderr, ") ");
        break;
    default:
        log_error("invalid node type %d\n", node->type);
        exit(-1);
    }
}

int parse(struct compile_process* process)
{
    log_debug("is called\n");
    current_compiler_process = process;
    parser_last_token = NULL;
    node_set_vector(process->node_vec, process->node_tree_vec);

    struct node* node;
    while (parse_next() == 0)
    {
        node = node_peek_or_null();

        // core
        vector_push(process->node_tree_vec, &node);

        log_info("\n");
    }

    // debug
    log_info("\n");
    log_info("parse tokens complete, try to read nodes\n\n");
    struct vector* vector = process->node_vec;
    vector_set_peek_pointer(vector, 0);
    struct node** node_ptr = vector_peek(vector);
    while (node_ptr)
    {
        read_nodes(*node_ptr);
        node_ptr = vector_peek(vector);
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
    vector_set_peek_pointer(vector, 0);
    return PARSE_ALL_OK;
}