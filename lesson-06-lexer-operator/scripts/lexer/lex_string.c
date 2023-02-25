#include "lexer.h"
#include "lexer/lex_helper.h"

#include <assert.h>
#include <string.h>

#include "pos.h"
#include "token.h"
#include "helpers/buffer.h"
#include "helpers/logger.h"



struct token* token_make_string(char start_delim, char end_delim)
{
    struct logger* logger = get_logger("lex_string.c", "token_make_string");
    struct buffer* buffer = buffer_create();
    struct pos pos = *lex_process_pos();

    assert(nextc() == start_delim);

    for (char c = nextc(); c != end_delim; c = nextc())
    {
        // case end delimiter not found
        if (c == EOF)
        {
            buffer_free(buffer);
            vector_free(lexer_tokens());
            char_not_found_error(logger, pos, c);
        }

        if (c == '\\')
        {
            // cases like \n
            continue;
        }

        buffer_write(buffer, c);
    }
    buffer_write(buffer, 0x00);
    
    struct token* token = token_create(&(struct token){
        .type = TOKEN_TYPE_STRING,
        .sval = strdup(buffer_ptr(buffer))
    });
    buffer_free(buffer);

    return token;
}