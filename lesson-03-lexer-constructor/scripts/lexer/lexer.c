#include "lexer.h"

#include "helpers/logger.h"

int lex(struct lex_process* process)
{
    struct logger* logger = get_logger("lexer.c", "lex");
    return LEXICAL_ANALYSIS_ALL_OK;
}