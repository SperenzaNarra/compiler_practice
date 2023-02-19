#include "lexer.h"

#include "helpers/logger.h"

int lex(struct lex_process* process)
{
    struct logger* logger = get_logger("lexer.c");
    logger->debug(logger, "lex() is called\n");
    return LEXICAL_ANALYSIS_ALL_OK;
}