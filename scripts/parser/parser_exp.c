#include "parser.h"
#include "parser/parse_helper.h"

void parse_expression_normal(struct history* history);

int parse_exp(struct history* history)
{
    parse_expression_normal(history);
    return 0;
}

int parse_expressionable_single(struct history* history)
{
    struct token* token = token_peek_next();
    if (!token) return -1;
    history->flags |= NODE_FLAG_INSIDE_EXPRESSION;

    int res;
    switch (token->type)
    {
        case TOKEN_TYPE_NUMBER:
            parse_single_token_to_node();
            res = 0;
            break;

        case TOKEN_TYPE_OPERATOR:
            res = parse_exp(history);
            res = 0;
            break;

        default:
            res = -1;
            break;
    }
}

void parse_expressionable(struct history* history)
{
    while(parse_expressionable_single(history) == 0)
    {

    }
}

void parse_expressionable_for_op(struct history* history, const char* op)
{
    parse_expressionable(history);
}

struct node* make_exp_node(struct node* left, struct node* right, const char* op)
{
    if (!left || !right)
    {
        log_error("get invalid argument\n");
        log_error("left: %p\nright: %p\n", left, right);
        exit(-1);
    }

    return node_create(&(struct node){
        .type = NODE_TYPE_EXPRESSION,
        .exp.left = left,
        .exp.right = right,
        .exp.op = op
    });

}

void parse_expression_normal(struct history* history)
{
    struct token* token = token_peek_next();
    if (token->type != TOKEN_TYPE_OPERATOR) return;

    char* op = token->sval;
    struct node* node_left = node_peek_expressionable_or_null();
    if (!node_left) return;

    // pop operator token
    token_next();

    // pop left node
    node_pop();

    node_left->flags |= NODE_FLAG_INSIDE_EXPRESSION; 
    parse_expressionable_for_op(history_down(history, history->flags), op);

    struct node* node_right = node_pop();
    node_right->flags |= NODE_FLAG_INSIDE_EXPRESSION;

    make_exp_node(node_left, node_right, op);
    struct node* exp_node = node_pop();

    // reorder expression
    node_push(exp_node);

    exit(-1);

}