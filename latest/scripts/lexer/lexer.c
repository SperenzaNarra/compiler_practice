#include "lexer.h"

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


static struct token* read_next_token()
{
    struct logger* logger = get_logger("lexer.c");
    logger->debug(logger, "read_next_token() is called\n");

    struct token* token = NULL;
    char c = peekc();
    switch (c)
    {
    case '0' ... '9':
        // number case
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

int lex(struct lex_process* process)
{
    struct logger* logger = get_logger("lexer.c");
    logger->debug(logger, "lex() is called\n");

    lex_process = process;

    struct token* token = read_next_token();
    while (token)
    {
        vector_push(process->token_vec, token);
        token = read_next_token();
    }

    return LEXICAL_ANALYSIS_ALL_OK;
}