#include "lexer.h"
#include "lexer/lex_helper.h"

#include <string.h>

#include "token.h"
#include "pos.h"
#include "helpers/logger.h"
#include "helpers/buffer.h"

struct token* token_make_identifier_or_keyword()
{
    
    struct pos pos = *lex_process_pos();
 
    struct buffer* buffer = buffer_create();
    char c;
    LEX_GETC_IF(buffer, c, 
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') ||
        (c == '_')
    );
    buffer_write(buffer, 0x00);

    char* str = strdup(buffer_ptr(buffer));
    buffer_free(buffer);

    // check if this is a keyword
    if (is_keyword(str))
    {
        return token_create(&(struct token){
            .type = TOKEN_TYPE_KEYWORD,
            .sval = str,
            .pos = pos
        });
    }

    // otherwise
    return token_create(&(struct token){
        .type = TOKEN_TYPE_IDENTIFIER,
        .sval = str,
        .pos = pos
    });
}