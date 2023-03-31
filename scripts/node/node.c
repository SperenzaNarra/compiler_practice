#include "node.h"

#include <string.h>

#include "pos.h"
#include "helpers/logger.h"
#include "helpers/vector.h"

struct vector* node_vector;
struct vector* node_vector_root;

void node_set_vector(struct vector* vec, struct vector* vec_root)
{
    node_vector = vec;
    node_vector_root = vec_root;
}

void node_push(struct node* node)
{
    log_debug("get node\n");
    read_node(node);
    vector_push(node_vector, node);
}

struct node* node_peek_or_null()
{
    return vector_back_ptr_or_null(node_vector);
}

struct node* node_peek()
{
    return *(struct node**)vector_back(node_vector);
}

struct node* node_pop()
{
    struct node* last_node = vector_back_ptr(node_vector);
    struct node* last_node_root = vector_empty(node_vector) ? NULL : vector_back_ptr_or_null(node_vector_root);

    vector_pop(node_vector);

    if (last_node == last_node_root)
    {
        vector_pop(node_vector_root);
    }
    
    return last_node;
}

struct node* node_create(struct node* _node)
{
    struct node* node = malloc(sizeof(struct node));
    memcpy(node, _node, sizeof(struct node));
    #warning "We should set the binded owner and binded function here"
    node_push(node);
    return node;
}

static bool node_is_expressionable(struct node* node)
{
    return 
        node->type == NODE_TYPE_EXPRESSION ||
        node->type == NODE_TYPE_EXPRESSION_PARENTHESES ||
        node->type == NODE_TYPE_UNARY ||
        node->type == NODE_TYPE_IDENTIFIER ||
        node->type == NODE_TYPE_NUMBER ||
        node->type == NODE_TYPE_STRING;
}

struct node* node_peek_expressionable_or_null()
{
    struct node* last_node = node_peek_or_null();
    log_debug("peek a node\n");
    if (last_node) read_node(last_node);
    return node_is_expressionable(last_node) ? last_node : NULL;
}

// default is printing cval value
static char* node_type_to_str(struct node* node)
{
    switch (node->type)
    {
    case NODE_TYPE_EXPRESSION: 
        log_info("  type : Expression\n");
        log_info("  val : %s\n", node->sval);
        log_info("  left : %p\n", node->exp.left);
        log_info("  op : %c\n", node->exp.op);
        log_info("  right : %p\n", node->exp.right);
        break;
    case NODE_TYPE_EXPRESSION_PARENTHESES: 
        log_info("  type : Expression Parentheses\n");
        log_info("  val : %c\n", node->cval);
        break;

    case NODE_TYPE_NUMBER: 
        log_info("  type : Number\n");
        log_info("  val : %lld\n", node->llnum);
        break;
    case NODE_TYPE_IDENTIFIER: 
        log_info("  type : Identifier\n");
        log_info("  val : %s\n", node->sval);
        break;
    case NODE_TYPE_STRING: 
        log_info("  type : String\n");
        log_info("  val : %s\n", node->sval);
        break;

    case NODE_TYPE_VARIABLE: 
        log_info("  type : Variable\n");
        log_info("  val : %s\n", node->sval);
        break;
    case NODE_TYPE_VARIABLE_LIST: 
        log_info("  type : Variable List\n");
        log_info("  val : %c\n", node->cval);
        break;

    case NODE_TYPE_FUNCTION: 
        log_info("  type : Function\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_BODY: 
        log_info("  type : Body\n");
        log_info("  val : %c\n", node->cval);
        break;

    
    case NODE_TYPE_STATEMENT_RETURN: 
        log_info("  type : Return\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_IF: 
        log_info("  type : If\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_ELSE: 
        log_info("  type : Else\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_WHILE: 
        log_info("  type : While\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_DO_WHILE: 
        log_info("  type : Do While\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_FOR: 
        log_info("  type : For\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_BREAK: 
        log_info("  type : Break\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_CONTINUE: 
        log_info("  type : Continue\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_SWITCH: 
        log_info("  type : Switch\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_CASE: 
        log_info("  type : Case\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_DEFAULT: 
        log_info("  type : Default\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STATEMENT_GOTO: 
        log_info("  type : Goto\n");
        log_info("  val : %c\n", node->cval);
        break;

    case NODE_TYPE_UNARY:
        log_info("  type : Unary\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_TENARY:
        log_info("  type : Tenary\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_LABEL:
        log_info("  type : Label\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_STRUCT:
        log_info("  type : Struct\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_UNION:
        log_info("  type : Union\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_BRACKET:
        log_info("  type : Bracket\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_CAST:
        log_info("  type : Cast\n");
        log_info("  val : %c\n", node->cval);
        break;
    case NODE_TYPE_BLANK:
        log_info("  type : Blank\n");
        log_info("  val : %c\n", node->cval);
        break;
    
    default: 
        log_info("  type : Known\n");
        log_info("  val : %c\n", node->cval);
        break;
    }
}

void read_node_binded(struct node_binded* node_binded)
{
    log_info("  owner : %p\n", node_binded->owner);
    log_info("  function : %p\n", node_binded->function);
}

void read_node(struct node* node)
{
    node_type_to_str(node);
    read_node_binded(&node->binded);
    read_pos(&node->pos);
}