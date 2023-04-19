#include "lexer.h"
#include "lexer/lex_helper.h"

#include <stdarg.h>
#include <ctype.h>

#include "helpers/logger.h"
#include "helpers/vector.h"

void lex_error(struct pos pos, const char* msg, ...)
{
    char buffer[128];
    log_error("lexical error\n");

    va_list args;
    va_start(args, msg);
    vsprintf(buffer, msg, args);
    log_error("%s", buffer);
    va_end(args);

    log_error("on line %d col %d in file %s\n", pos.line, pos.col, pos.filename);
    exit(-1);
}

struct vector* lexer_tokens()
{
    return lex_process->token_vec;
}

char nextc()
{
    char c = lex_process->next_char(lex_process);
    char* str = display_char(c);
    log_debug("get char %s (line %d col %d)\n", str, lex_process->pos.line, lex_process->pos.col);
    free(str);

    lex_process->last_pos = lex_process->pos;
    lex_process->pos.col += 1;
    

    if (c == '\n')
    {
        lex_process->pos.line += 1;
        lex_process->pos.col = 1;
    }

    if (lex_is_in_expression())
    {
        buffer_write(lex_process->parenthesis_buffer, c);
    }
    
    

    return c;
}

char peekc()
{
    char c = lex_process->peek_char(lex_process);
    char* str = display_char(c);
    log_debug("get char %s (line %d col %d)\n", str, lex_process->pos.line, lex_process->pos.col);
    free(str);

    return c;
}

void pushc(char c)
{
    if (lex_is_in_expression())
    {
        struct buffer* buffer = lex_process->parenthesis_buffer;
        if (buffer->len > 0)
        {
            buffer->data[buffer->len - 1] = 0x00; 
            buffer->len--;
        }
    }
    lex_process->push_char(lex_process, c);
    lex_process->pos = lex_process->last_pos;
    char* str = display_char(c);
    log_debug("get char %s (line %d col %d)\n", str, lex_process->pos.line, lex_process->pos.col);
    free(str);
}


void lexer_pop_token()
{
    log_debug("is called\n");
    vector_pop(lex_process->token_vec);
}

void lex_new_expression()
{
    log_debug("is called\n");
    lex_process->current_expression_count++;
    if (lex_process->current_expression_count == 1)
    {
        lex_process->parenthesis_buffer = buffer_create();
    }
}

void lex_finish_expression()
{
    log_debug("is called\n");
    lex_process->current_expression_count--;
    if (lex_process->current_expression_count < 0)
    {
        lex_error(lex_process->pos, "You closed an expression that you never open\n");
    }
}

bool lex_is_in_expression()
{
    return lex_process->current_expression_count > 0;
}

struct token* handle_whitespace()
{
    log_debug("is called\n");
    struct token* last_token = lexer_last_token();
    if (last_token) last_token->whitespace = true;
    nextc();
    return read_next_token();
}

struct token* read_special_token()
{
    log_debug("is called\n");
    char c = peekc();

    if (isalpha(c) || c == '_') return token_make_identifier_or_keyword();
    return NULL;
}

struct token* token_make_newline()
{
    log_debug("is called\n");
    struct pos pos = *lex_process_pos();
    nextc();
    return token_create(&(struct token){.type=TOKEN_TYPE_NEWLINE, .pos=pos});
}

struct token* read_next_token()
{
    log_debug("is called\n");
    struct token* token = NULL;
    char c = peekc();
    switch (c)
    {
    case NUMERIC_CASE:
        token = token_make_number();
        break;
    case '"':
        token = token_make_string(c, c);
        break;
    case '\'':
        token = token_make_quote();
        break;
    case OPERATOR_CASE_EXCLUDE_DIVISION:
        token = token_make_operator_or_string();
        break;
    case SYMBOL_CASE:
        token = token_make_symbol();
        break;
    case '/':
        token = token_make_comment();
        break;
    case SPECIAL_NUMBER_CASE:
        token = token_make_special_number();
        break;
    case '\n':
        token = token_make_newline();
        break;
    case ' ':
        token = handle_whitespace();
        break;

    case EOF:
        break;

    default:
        token = read_special_token();
        if (!token)
        {
            lex_error(lex_process->pos, "get unknown char %s\n", display_char(c));
        }
    }

    return token;
}

int lex(struct lex_process* process)
{
    log_debug("is called\n");
    lex_process = process;

    struct token* token = read_next_token();
    while (token)
    {
        vector_push(process->token_vec, token);
        token = read_next_token();
    }

    // check correction
    log_info("\n");
    log_info("lexical analysis complete, reading token vector\n");
    log_info("\n");
    token = vector_peek(lex_process->token_vec);
    while (token)
    {
        read_token(token);    
        token = vector_peek(lex_process->token_vec);
    }
    vector_set_peek_pointer(lex_process->token_vec, 0);

    return LEXICAL_ANALYSIS_ALL_OK;
}