#include "lexer.h"
#include "lexer/lex_helper.h"

#include <stdbool.h>
#include <string.h>

#include "token.h"
#include "pos.h"
#include "helpers/logger.h"
#include "helpers/buffer.h"

static bool op_treated_as_one(char op)
{
    return  op == '(' ||
            op == '[' ||
            op == ',' ||
            op == '.' ||
            op == '*' ||
            op == '?';
}

static bool is_single_operator(char op)
{
    return  op == '+' ||
            op == '-' ||
            op == '*' ||
            op == '/' ||
            op == '=' ||
            op == '>' ||
            op == '<' ||
            op == '|' ||
            op == '&' ||
            op == '^' ||
            op == '%' ||
            op == '~' ||
            op == '!' ||
            op == '?' ||
            op == '(' ||
            op == '[' ||
            op == ',' ||
            op == '.';
}

static bool op_valid(const char* op)
{
    return  S_EQ(op, "+") ||
            S_EQ(op, "-") ||
            S_EQ(op, "*") ||
            S_EQ(op, "%") ||
            S_EQ(op, "/") ||
            S_EQ(op, "=") ||
            S_EQ(op, "!") ||
            S_EQ(op, "~") ||
            S_EQ(op, "?") ||
            S_EQ(op, "^") ||
            S_EQ(op, ">") ||
            S_EQ(op, "<") ||
            S_EQ(op, "|") ||
            S_EQ(op, "&") ||
            S_EQ(op, "(") ||
            S_EQ(op, "[") ||
            S_EQ(op, ",") ||
            S_EQ(op, ".") ||
            S_EQ(op, "+=") ||
            S_EQ(op, "-=") ||
            S_EQ(op, "*=") ||
            S_EQ(op, "/=") ||
            S_EQ(op, ">>") ||
            S_EQ(op, "<<") ||
            S_EQ(op, ">=") ||
            S_EQ(op, "<=") ||
            S_EQ(op, "==") ||
            S_EQ(op, "!=") ||
            S_EQ(op, "||") ||
            S_EQ(op, "&&") ||
            S_EQ(op, "++") ||
            S_EQ(op, "--") ||
            S_EQ(op, "->") ||
            S_EQ(op, "...");
}

static void flush_back(struct buffer* buffer, struct pos pos)
{
    // flush back all char in buffer but first
    
    const char* data = buffer_ptr(buffer);
    int len = buffer->len;

    for (int i = len-1; i > 0; i--)
    {
        if (data[i] == 0x00) continue;
        pushc(data[i]);
    }

    lex_process_set_pos(&pos);
}

static char* read_operator()
{
    

    bool single_operator = true;
    char op = nextc();
    struct pos pos = *lex_process_pos();

    struct buffer* buffer = buffer_create();
    buffer_write(buffer, op);

    if (!op_treated_as_one(op))
    {
        // ++ += 
        op = peekc();
        if (is_single_operator(op))
        {
            // pop it
            buffer_write(buffer, op);
            nextc();
            single_operator = false;
        }
    }
    buffer_write(buffer, 0x00);

    char* ptr = buffer_ptr(buffer);
    if (!single_operator)
    {
        if (!op_valid(ptr))
        {
            flush_back(buffer, pos);
            ptr[1] = 0x00;
        }
    }
    else if(!op_valid(ptr))
    {
        lex_error(pos, "The operator %s is not valid\n", ptr);
    }
    
    char* str = strdup(ptr);
    buffer_free(buffer);
    return str;
}

struct token* token_make_operator_or_string()
{
    
    char op = peekc();
    struct pos pos = *lex_process_pos();
    if (op == '<')
    {
        // #include <stdio.h>
        struct token* last_token = lexer_last_token();
        if (token_is_keyword(last_token, "include"))
        {
            return token_make_string('<', '>');
        }
    }

    struct token* token = token_create(&(struct token){
        .type = TOKEN_TYPE_OPERATOR,
        .sval = read_operator(),
        .pos = pos
    });

    if (op == '(')
    {
        lex_new_expression();
    }

    return token;
}