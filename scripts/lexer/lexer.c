#include "lexer.h"
#include "lexer/lex_helper.h"

#include <stdarg.h>
#include <ctype.h>

#include "helpers/logger.h"
#include "helpers/vector.h"

void lex_error(struct logger* logger, struct pos pos, const char* msg, ...)
{
    char buffer[128];
    logger->error(logger, "lexical error\n");

    va_list args;
    va_start(args, msg);
    vsprintf(buffer, msg, args);
    logger->error(logger, "%s", buffer);
    va_end(args);

    logger->error(logger, "on line %d col %d in file %s\n", pos.line, pos.col, pos.filename);
    exit(-1);
}

struct vector* lexer_tokens()
{
    return lex_process->token_vec;
}

char nextc()
{
    struct logger* logger = get_logger("lexer.c", "nextc");
    
    char c = lex_process->next_char(lex_process);
    char* str = display_char(c);
    logger->debug(logger, "get char %s (line %d col %d)\n", str, lex_process->pos.line, lex_process->pos.col);
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
    struct logger* logger = get_logger("lexer.c", "peekc");

    char c = lex_process->peek_char(lex_process);
    char* str = display_char(c);
    logger->debug(logger, "get char %s (line %d col %d)\n", str, lex_process->pos.line, lex_process->pos.col);
    free(str);

    return c;
}

void pushc(char c)
{
    struct logger* logger = get_logger("lexer.c", "pushc");

    char* str = display_char(c);
    logger->debug(logger, "get char %s\n", str);
    free(str);

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
}


void lexer_pop_token()
{
    vector_pop(lex_process->token_vec);
}

void lex_new_expression()
{
    struct logger* logger = get_logger("lexer.c", "lex_new_expression");
    lex_process->current_expression_count++;
    if (lex_process->current_expression_count == 1)
    {
        lex_process->parenthesis_buffer = buffer_create();
    }
}

void lex_finish_expression()
{
    struct logger* logger = get_logger("lexer.c", "lex_finish_expression");
    lex_process->current_expression_count--;
    if (lex_process->current_expression_count < 0)
    {
        lex_error(logger, lex_process->pos, "You closed an expression that you never open\n");
    }
}

bool lex_is_in_expression()
{
    return lex_process->current_expression_count > 0;
}

struct token* handle_whitespace()
{
    struct logger* logger = get_logger("lexer.c", "handle_whitespace");

    struct token* last_token = lexer_last_token();
    if (last_token) last_token->whitespace = true;
    nextc();
    return read_next_token();
}

struct token* read_special_token()
{
    struct logger* logger = get_logger("lexer.c", "read_special_token");
    char c = peekc();

    if (isalpha(c) || c == '_') return token_make_identifier_or_keyword();
    return NULL;
}

struct token* token_make_newline()
{
    struct pos pos = *lex_process_pos();
    nextc();
    return token_create(&(struct token){.type=TOKEN_TYPE_NEWLINE, .pos=pos});
}

struct token* read_next_token()
{
    struct logger* logger = get_logger("lexer.c", "read_next_token");

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
            lex_error(logger, lex_process->pos, "get unknown char %s\n", display_char(c));
        }
    }

    return token;
}

int lex(struct lex_process* process)
{
    struct logger* logger = get_logger("lexer.c", "lex");

    lex_process = process;

    struct token* token = read_next_token();
    while (token)
    {
        vector_push(process->token_vec, token);
        token = read_next_token();
    }

    return LEXICAL_ANALYSIS_ALL_OK;
}