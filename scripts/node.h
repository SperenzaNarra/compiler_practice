#ifndef NODE_H
#define NODE_H

/** this header is for parse analysis **/

#include "pos.h"

#include "helpers/vector.h"

enum
{
    NODE_FLAG_INSIDE_EXPRESSION = 0b00000001,
};

enum 
{
    NODE_TYPE_UNKOWN,

    NODE_TYPE_EXPRESSION,
    NODE_TYPE_EXPRESSION_PARENTHESES,

    NODE_TYPE_NUMBER,
    NODE_TYPE_IDENTIFIER,
    NODE_TYPE_STRING,

    NODE_TYPE_VARIABLE,
    NODE_TYPE_VARIABLE_LIST,

    NODE_TYPE_FUNCTION,
    NODE_TYPE_BODY,

    NODE_TYPE_STATEMENT_RETURN,
    NODE_TYPE_STATEMENT_IF,
    NODE_TYPE_STATEMENT_ELSE,
    NODE_TYPE_STATEMENT_WHILE,
    NODE_TYPE_STATEMENT_DO_WHILE,
    NODE_TYPE_STATEMENT_FOR,
    NODE_TYPE_STATEMENT_BREAK,
    NODE_TYPE_STATEMENT_CONTINUE,
    NODE_TYPE_STATEMENT_SWITCH,
    NODE_TYPE_STATEMENT_CASE,
    NODE_TYPE_STATEMENT_DEFAULT,
    NODE_TYPE_STATEMENT_GOTO,

    NODE_TYPE_UNARY,
    NODE_TYPE_TENARY,
    NODE_TYPE_LABEL,
    NODE_TYPE_STRUCT,
    NODE_TYPE_UNION,
    NODE_TYPE_BRACKET,
    NODE_TYPE_CAST,
    NODE_TYPE_BLANK,
};

struct node;
struct node_binded
{
    // point to the body node
    struct node* owner;

    // point to the function
    struct node* function;

};

struct expression
{
    struct node* left;
    struct node* right;
    const char* op;
};

struct node
{
    int type;
    int flags;
    
    struct pos pos;
    struct node_binded binded;

    union
    {
        char cval;
        const char* sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void* any;
        struct expression exp;
    };
    
};

struct node* node_create(struct node* _node);
struct node* node_peek_expressionable_or_null();
void node_set_vector(struct vector* vec, struct vector* vec_root);
void node_push(struct node* node);
struct node* node_peek_or_null();
struct node* node_peek();
struct node* node_pop();
struct node* node_peek();

void read_node(struct node* node);
void read_node_simple(struct node* node);

#endif // NODE_H