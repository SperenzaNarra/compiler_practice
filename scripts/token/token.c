#include "token.h"

#include <string.h>

#include "lexer.h"
#include "helpers/logger.h"

bool token_is_keyword(struct token* token, const char* keyword)
{
    return token->type == TOKEN_TYPE_KEYWORD && (strcmp(token->sval, keyword) == 0);
}

static void read_pos(struct pos* pos)
{
#ifdef INFO_TOKEN
    struct logger* logger = get_logger("token.c", "read_pos");
    logger->debug(logger, "read pos\n");
    logger->info(logger, "  col  : %d\n", pos->col);
    logger->info(logger, "  line : %d\n", pos->line);
    logger->info(logger, "  filename : %s\n", pos->filename);
#endif
}

static void read_token_number(struct logger* logger, struct token_number* token)
{
#ifdef INFO_TOKEN
    switch (token->type)
    {
    case NUMBER_TYPE_NORMAL:
        logger->info(logger, "  type  : normal number\n");
        break;
    case NUMBER_TYPE_LONG:
        logger->info(logger, "  type  : long number\n");
        break;
    case NUMBER_TYPE_FLOAT:
        logger->info(logger, "  type  : float number\n");
        break;
    case NUMBER_TYPE_DOUBLE:
        logger->info(logger, "  type  : double number\n");
        break;
    default:
        logger->info(logger, "  type  : unknown number\n");
        break;
    }
#endif
}

void read_token(struct token* token)
{
#ifdef INFO_TOKEN
    struct logger* logger = get_logger("token.c", "read_token");
    logger->debug(logger, "read token\n");

    switch (token->type)
    {
    case TOKEN_TYPE_IDENTIFIER:
        logger->info(logger, "  type  : identifier\n");
        logger->info(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_KEYWORD:
        logger->info(logger, "  type  : keyword\n");
        logger->info(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_OPERATOR:
        logger->info(logger, "  type  : operator\n");
        logger->info(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_SYMBOL:
        logger->info(logger, "  type  : symbol\n");
        logger->info(logger, "  value : %c\n", token->cval);
        break;
    case TOKEN_TYPE_NUMBER:
        read_token_number(logger, &token->num);
        logger->info(logger, "  value : %lld\n", token->llnum);
        break;
    case TOKEN_TYPE_STRING:
        logger->info(logger, "  type  : string\n");
        logger->info(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_COMMENT:
        logger->info(logger, "  type  : comment\n");
        logger->info(logger, "  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_NEWLINE:
        logger->info(logger, "  type  : new line\n");
        break;
    };
    logger->info(logger, "  whitespace : %s\n", token->whitespace ? "true" : "false");
    logger->info(logger, "  between_brackets : %s\n", token->between_brackets);
    read_pos(&token->pos);
    logger->info(logger, "\n");
#endif
}


struct token tmp_token;

struct token* token_create(struct token* _token)
{
    struct logger* logger = get_logger("token.c", "token_create");
    memcpy(&tmp_token, _token, sizeof(struct token));

    if (lex_is_in_expression())
    {
        tmp_token.between_brackets = buffer_ptr(lex_parenthesis_buffer());
    }

    read_token(&tmp_token);

    return &tmp_token;
}