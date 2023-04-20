#include "parser.h"
#include "parser/parse_helper.h"

#include "history.h"
#include "token.h"

static struct expressionable_op_precedence_group op_precedence[TOTAL_OPERATOR_GROUPS] = 
{   
    { .operators = {"++", "--", "()", "[]", "(", "[", ".", "->", NULL},                         .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"*", "/", "%", NULL},                                                       .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"+", "-", NULL},                                                            .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"<<", ">>", NULL},                                                          .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"<", "<=", ">", ">=", NULL},                                                .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"==", "!=", NULL},                                                          .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"&", NULL},                                                                 .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"^", NULL},                                                                 .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"|", NULL},                                                                 .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"&&", NULL},                                                                .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"||", NULL},                                                                .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
    { .operators = {"?", ":", NULL},                                                            .associtivity = ASSOCIATIVITY_RIGHT_TO_LEFT },
    { .operators = {"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", NULL},   .associtivity = ASSOCIATIVITY_RIGHT_TO_LEFT },
    { .operators = {",", NULL},                                                                 .associtivity = ASSOCIATIVITY_LEFT_TO_RIGHT },
};

static int parse_expressionable_single(struct history* history);

static void parse_expressionable_for_op(struct history* history, char* op)
{
    log_debug("is called\n");
    parse_expressionable(history);

}

void make_exp_node(struct node* left_node, struct node* right_node, const char* op)
{
    node_create(&(struct node)
    {
        .type = NODE_TYPE_EXPRESSION,
        .pos = left_node->pos,
        .exp.left = left_node,
        .exp.right = right_node,
        .exp.op = op,
    });
}

static int get_op_presidence(const char* op, struct expressionable_op_precedence_group** group_out)
{
    *group_out = NULL;
    for (int i = 0; i < TOTAL_OPERATOR_GROUPS; i++)
    for (int j = 0; op_precedence[i].operators[j]; j++)
    {
        const char* _op = op_precedence[i].operators[j];
        if (S_EQ(op, _op))
        {
            *group_out = &op_precedence[i];
            return i;
        }
    }

    log_error("unkown operator %s\n", op);
    exit(-1);
}

static bool parser_exp_left_has_priority(const char* left_op, const char* right_op)
{
    log_debug("is called\n");

    struct expressionable_op_precedence_group* left_group  = NULL;
    struct expressionable_op_precedence_group* right_group = NULL;

    if (S_EQ(left_op, right_op)) return false;

    int left_precidence  = get_op_presidence(left_op, &left_group);
    int right_precidence = get_op_presidence(right_op, &right_group);

    log_info("get left precidence %d with operator %s\n", left_precidence, left_op);
    log_info("get left precidence %d with operator %s\n", right_precidence, right_op);

    if (left_group->associtivity == ASSOCIATIVITY_RIGHT_TO_LEFT) return false;
    return left_precidence <= right_precidence;
}

static void parser_exp_shift_children_left(struct node* node)
{
    // 50 * 30  + 20 -> 50 * (30 + 20)

    log_debug("is called\n");
    if (node->type != NODE_TYPE_EXPRESSION)
    {
        log_error("invalid node type\n");
        read_node(node);
        exit(-1);
    }
    if (node->exp.right->type != NODE_TYPE_EXPRESSION)
    {
        log_error("invalid node type of right node\n");
        read_node(node->exp.right);
        exit(-1);
    }

    // +
    const char* right_op = node->exp.right->exp.op;
    // 50
    struct node* new_exp_left_node  = node->exp.left;
    // 30
    struct node* new_exp_right_node = node->exp.right->exp.left;

    make_exp_node(new_exp_left_node, new_exp_right_node, node->exp.op);

    // (50 * 30)
    struct node* new_left_operand  = node_pop();
    // 20
    struct node* new_right_operand = node->exp.right->exp.right;

    free(node->exp.right);

    // (50 * 30) + 20
    node->exp.left = new_left_operand;
    node->exp.right = new_right_operand;
    node->exp.op = right_op;
}

static void parser_reorder_expression(struct node** node_ptr)
{
    log_debug("is called\n");

    struct node* node = *node_ptr;
    if (node->type != NODE_TYPE_EXPRESSION) return;
    
    struct node* left_node  = node->exp.left;
    struct node* right_node = node->exp.right;

    if (!left_node)
    {
        log_error("cannot find left node\n");
        read_node(node);
        exit(-1);
    }
    if (!right_node) return;

    bool left_is_exp  = left_node->type  == NODE_TYPE_EXPRESSION;
    bool right_is_exp = right_node->type == NODE_TYPE_EXPRESSION;

    if (!left_is_exp && !right_is_exp) return;
    if (!left_is_exp && right_is_exp)
    {
        const char* right_op = right_node->exp.op;
        if (parser_exp_left_has_priority(node->exp.op, right_op))
        {
            parser_exp_shift_children_left(node);
            parser_reorder_expression(&node->exp.left);
            parser_reorder_expression(&node->exp.right);
        }
    }

}

static void parse_exp_normal(struct history* history)
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
    struct node* left_node = node_peek_expressionable_or_null();
    if (!left_node)
    {
        log_error("left node of expression not found\n");
        log_error("at line %d col %d\n", op_token->pos.line, op_token->pos.col);
        exit(-1);
    }
    left_node->flags |= NODE_FLAG_INSIDE_EXPRESSION;
    read_node(left_node);

    // pop off the operator token
    parse_next_token();

    // pop off the left node;
    node_pop();

    log_debug("get right node\n");
    parse_expressionable_for_op(history_down(history, history->flags), op_token->sval);

    struct node* right_node = node_peek_expressionable_or_null();

    if (right_node)
    {
        right_node->flags |= NODE_FLAG_INSIDE_EXPRESSION;
        read_node(right_node);
    
        // pop off the right node
        node_pop();
    }

    make_exp_node(left_node, right_node, op_token->sval);

    struct node* exp_node = node_pop();
    parser_reorder_expression(&exp_node);
    read_node(exp_node);
    node_push(exp_node);
}

static void parse_exp(struct history* history)
{
    log_debug("is called\n");
    history->last_type = HISTORY_LAST_TYPE_NONE;
    parse_exp_normal(history);
}

static int parse_expressionable_single(struct history* history)
{
    log_debug("is called\n");
    if (history->last_type == HISTORY_LAST_TYPE_END) return -1;

    struct token* token = parse_peek_token();
    if (!token) return -1;

    history->flags |= NODE_FLAG_INSIDE_EXPRESSION;
    
    int res = -1;

    // in case of parsing "1+2 3+4" -> (1+2) (3+4)

    switch (token->type)
    {
    case TOKEN_TYPE_NUMBER:
    case TOKEN_TYPE_STRING:
    case TOKEN_TYPE_IDENTIFIER:
        if (history->last_type == HISTORY_LAST_TYPE_VALUE)
        {
            history->last_type = HISTORY_LAST_TYPE_END;
            break;
        }
        history->last_type = HISTORY_LAST_TYPE_VALUE;
        parse_single_token_to_node();
        res = 0;
        break;
        
    case TOKEN_TYPE_OPERATOR:
        history->last_type = HISTORY_LAST_TYPE_OPERATOR;
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