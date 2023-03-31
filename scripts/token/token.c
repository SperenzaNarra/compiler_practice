#include "token.h"

#include <string.h>

#include "lexer.h"
#include "helpers/logger.h"

bool token_is_keyword(struct token* token, const char* keyword)
{
    return token->type == TOKEN_TYPE_KEYWORD && (strcmp(token->sval, keyword) == 0);
}

void read_pos(struct pos* pos)
{
    log_debug("read pos\n");
    log_info("  col  : %d\n", pos->col);
    log_info("  line : %d\n", pos->line);
    log_info("  filename : %s\n", pos->filename);
}

static void read_token_number(struct token_number* token_number)
{
    switch (token_number->type)
    {
    case NUMBER_TYPE_NORMAL:
        log_info("  type  : normal number\n");
        break;
    case NUMBER_TYPE_LONG:
        log_info("  type  : long number\n");
        break;
    case NUMBER_TYPE_FLOAT:
        log_info("  type  : float number\n");
        break;
    case NUMBER_TYPE_DOUBLE:
        log_info("  type  : double number\n");
        break;
    default:
        log_info("  type  : unknown number\n");
        exit(-1);
    }
}

void read_token(struct token* token)
{    
    log_debug("read token\n");

    switch (token->type)
    {
    case TOKEN_TYPE_IDENTIFIER:
        log_info("  type  : identifier\n");
        log_info("  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_KEYWORD:
        log_info("  type  : keyword\n");
        log_info("  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_OPERATOR:
        log_info("  type  : operator\n");
        log_info("  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_SYMBOL:
        log_info("  type  : symbol\n");
        log_info("  value : %c\n", token->cval);
        break;
    case TOKEN_TYPE_NUMBER:
        read_token_number(&token->num);
        log_info("  value : %lld\n", token->llnum);
        break;
    case TOKEN_TYPE_STRING:
        log_info("  type  : string\n");
        log_info("  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_COMMENT:
        log_info("  type  : comment\n");
        log_info("  value : %s\n", token->sval);
        break;
    case TOKEN_TYPE_NEWLINE:
        log_info("  type  : new line\n");
        break;
    default:
        log_info("  type  : unkown type\n");
        exit(-1);
    };
    log_info("  whitespace : %s\n", token->whitespace ? "true" : "false");
    log_info("  between_brackets : %s\n", token->between_brackets);
    read_pos(&token->pos);
    log_info("\n");
}


struct token tmp_token;

struct token* token_create(struct token* _token)
{
    
    memcpy(&tmp_token, _token, sizeof(struct token));

    if (lex_is_in_expression())
    {
        tmp_token.between_brackets = buffer_ptr(lex_parenthesis_buffer());
    }

    read_token(&tmp_token);

    return &tmp_token;
}