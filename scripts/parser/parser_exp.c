#include "parser.h"
#include "parser/parse_helper.h"

#include "history.h"
#include "token.h"

int parse_expressionable_single(struct history* history);

void parse_expressionable_for_op(struct history* history, char* op)
{
    log_debug("is called\n");
    parse_expressionable(history);

}

void parse_exp_normal(struct history* history)
{
    log_debug("is called\n");
    struct token* op_token = parse_peek_token();
    if (op_token->type != TOKEN_TYPE_OPERATOR)
    {
        log_error("invalid token type %d from\n");
        read_token(op_token);
        exit(-1);
    }

    log_debug("get operator %s\n", op_token->sval);
    
    log_debug("get left node\n");
    struct node* node_left = node_peek_expressionable_or_null();
    if (!node_left)
    {
        log_error("left node of expression not found\n");
        log_error("at line %d col %d\n", op_token->pos.line, op_token->pos.col);
        exit(-1);
    }
    node_left->flags |= NODE_FLAG_INSIDE_EXPRESSION;
    read_node(node_left);

    // pop off the operator token
    parse_next_token();

    // pop off the left node;
    node_pop();

    log_debug("get right node\n");
    parse_expressionable_for_op(history_down(history, history->flags), op_token->sval);

    struct node* node_right = node_peek_expressionable_or_null();
    if (!node_right)
    {
        log_error("right node of expression not found\n");
        log_error("at line %d col %d\n", op_token->pos.line, op_token->pos.col);
        exit(-1);
    }

    node_right->flags |= NODE_FLAG_INSIDE_EXPRESSION;
    read_node(node_right);

    // pop off the right node
    node_pop();

    struct node* exp_node = node_create(&(struct node)
    {
        .type = NODE_TYPE_EXPRESSION,
        .exp.left = node_left,
        .exp.right = node_right,
        .exp.op = op_token->sval,
    });
}

void parse_exp(struct history* history)
{
    log_debug("is called\n");
    parse_exp_normal(history);
}

int parse_expressionable_single(struct history* history)
{
    log_debug("is called\n");
    struct token* token = parse_peek_token();
    if (!token) return -1;

    history->flags |= NODE_FLAG_INSIDE_EXPRESSION;
    
    int res = -1;

    switch (token->type)
    {
    case TOKEN_TYPE_NUMBER:
    case TOKEN_TYPE_STRING:
    case TOKEN_TYPE_IDENTIFIER:
        parse_single_token_to_node();
        res = 0;
        break;
        
    case TOKEN_TYPE_OPERATOR:
        parse_exp(history);
        res = 0;
        break;

    default:
        log_error("invalid token type %d from\n", token->type);
        read_token(token);
        break;
    }
    return res;
}

void parse_expressionable(struct history* history)
{
    log_debug("is called\n");
    while (parse_expressionable_single(history) == 0)
    {

    }

}