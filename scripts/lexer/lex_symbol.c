#include "lexer.h"
#include "lexer/lex_helper.h"

#include "token.h"
#include "pos.h"
#include "helpers/logger.h"

struct token* token_make_symbol()
{
    
    struct pos pos = *lex_process_pos();
    if (peekc() == ')') lex_finish_expression();

    char c = nextc();
    return token_create(&(struct token){
        .type = TOKEN_TYPE_SYMBOL,
        .cval = c,
        .pos = pos
    });
}