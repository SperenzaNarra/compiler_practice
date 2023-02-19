#include "token.h"

#include <string.h>
#include "helpers/logger.h"


static inline void read_pos(struct pos* pos)
{
    struct logger* logger = get_logger("token.c");
    logger->debug(logger, "read pos\n");
    logger->debug(logger, "  col  : %d\n", pos->col);
    logger->debug(logger, "  line : %d\n", pos->line);
    logger->debug(logger, "  filename : %s\n", pos->filename);
}

static inline void read_token(struct token* token)
{
    struct logger* logger = get_logger("token.c");
    logger->debug(logger, "read token\n");

    switch (token->type)
    {
    case TOKEN_TYPE_IDENTIFIER:
        logger->debug(logger, "  type  : identifier\n");
        logger->debug(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_KEYWORD:
        logger->debug(logger, "  type  : keyword\n");
        logger->debug(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_OPERATOR:
        logger->debug(logger, "  type  : operator\n");
        logger->debug(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_SYMBOL:
        logger->debug(logger, "  type  : symbol\n");
        logger->debug(logger, "  value : %c\n", token->cval);
        break;
    case TOKEN_TYPE_NUMBER:
        logger->debug(logger, "  type  : number\n");
        logger->debug(logger, "  value : %lld\n", token->llnum);
        break;
    case TOKEN_TYPE_STRING:
        logger->debug(logger, "  type  : string\n");
        logger->debug(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_COMMENT:
        logger->debug(logger, "  type  : comment\n");
        logger->debug(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_NEWLINE:
        logger->debug(logger, "  type  : new line");
        break;
    };
    logger->debug(logger, "  whitespace : %s\n", token->whitespace ? "true" : "false");
    logger->debug(logger, "  between_brackets : %s\n", token->between_brackets);
    read_pos(&token->pos);
}


struct token tmp_token;

struct token* token_create(struct token* _token)
{
    struct logger* logger = get_logger("token.c");
    logger->debug(logger, "token_create() is called\n");

    memcpy(&tmp_token, _token, sizeof(struct token));
    read_token(&tmp_token);

    return &tmp_token;
}