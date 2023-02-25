#include "lexer.h"
#include "lexer/lex_helper.h"

#include "token.h"
#include "pos.h"
#include "helpers/logger.h"

struct token* token_make_symbol()
{
    struct logger* logger = get_logger("lex_symbol.c", "token_make_symbol");
    struct pos pos = *lex_process_pos();
    char c = nextc();
    if (c == ')') lex_finish_expression();

    return token_create(&(struct token){
        .type = TOKEN_TYPE_SYMBOL,
        .cval = c,
        .pos = pos
    });
}