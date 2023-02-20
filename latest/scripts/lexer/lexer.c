#include "lexer.h"
#include "lexer/lex_helper.h"

#include "helpers/logger.h"
#include "helpers/vector.h"

struct lex_process* lex_process;

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

struct token* lexer_last_token()
{
    return vector_back_or_null(lex_process->token_vec);
}



static struct token* read_next_token()
{
    struct logger* logger = get_logger("lexer.c", "read_next_token");

    struct token* token = NULL;
    char c = peekc();
    switch (c)
    {
    case NUMERIC_CASE:
        token = token_make_number();
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
        exit(-1);
    }

    return token;
}

struct token* handle_whitespace()
{
    struct token* last_token = lexer_last_token();
    if (last_token) last_token->whitespace = true;
    nextc();
    return read_next_token();
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