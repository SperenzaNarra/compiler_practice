#include "lexer.h"
#include "lexer/lex_helper.h"

#include "helpers/logger.h"
#include "helpers/vector.h"

struct lex_process* lex_process;

struct vector* lexer_tokens()
{
    return lex_process->token_vec;
}

void char_not_found_error(struct logger* logger, struct pos pos, char c)
{
    char* str = display_char(c);
    logger->error(logger, "cannot read end delimiter %s\n", str);
    logger->error(logger, "on line %d col %d in file %s\n", pos.line, pos.col, pos.filename);
    free(str);
    kill_all_logger();
    exit(-1);
}

char nextc()
{
    return lex_process->next_char(lex_process);
}

char peekc()
{
    return lex_process->peek_char(lex_process);
}

void pushc(char c)
{
    lex_process->push_char(lex_process, c);
}



struct pos* lex_process_pos()
{
    return &lex_process->pos;
}

void lex_process_set_pos(struct pos* pos)
{
    lex_process->pos = *pos;
}

struct token* lexer_last_token()
{
    return vector_back_or_null(lex_process->token_vec);
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
        logger->error(logger, "You closed an expression that you never open");
        logger->error(logger, "on line %d col %d in file %s\n", lex_process->pos.col, lex_process->pos.line, lex_process->pos.filename);
        kill_all_logger();
        exit(-1);
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
    case STRING_CASE:
        token = token_make_string(c, c);
        break;
    case OPERATOR_CASE_EXCLUDE_DIVISION:
        token = token_make_operator_or_string();
        break;
    case SYMBOL_CASE:
        token = token_make_symbol();
        break;
    case ' ':
        token = handle_whitespace();
        break;

    case EOF:
        break;

    default:;
        char* str = display_char(c);
        logger->error(logger, "get unknown char %s\n", str);
        logger->error(logger, "on line %d col %d in file %s\n", lex_process->pos.col, lex_process->pos.line, lex_process->pos.filename);
        free(str);
        kill_all_logger();
        exit(-1);
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