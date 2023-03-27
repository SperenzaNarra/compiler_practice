#include "lexer.h"
#include "lexer/lex_helper.h"

#include <string.h>
#include <stdbool.h>

#include "token.h"
#include "pos.h"
#include "helpers/buffer.h"
#include "helpers/logger.h"

struct token* token_make_one_line_comment()
{
    struct buffer* buffer = buffer_create();
    struct pos pos = *lex_process_pos();

    char c;
    LEX_GETC_IF(buffer, c, c != '\n' && c != EOF);

    char* str = strdup(buffer_ptr(buffer));
    buffer_free(buffer);
    return token_create(&(struct token){
        .type = TOKEN_TYPE_COMMENT,
        .sval = str,
        .pos = pos
    });
}

struct token* token_make_multiline_comment()
{
    struct pos pos = *lex_process_pos();

    struct buffer* buffer = buffer_create();
    char c;

    while (true)
    {
        LEX_GETC_IF(buffer, c, 
            c != '*' &&
            c != EOF
        );

        c = nextc();
        if (c == EOF) lex_error(pos, "You didn't close this multiline comment\n");
        
        // expecting '*'
        if (peekc() == '/')
        {
            nextc();
            break;
        }
        buffer_write(buffer, '*');
    }

    char* str = strdup(buffer_ptr(buffer));
    buffer_free(buffer);
    return token_create(&(struct token){
        .type = TOKEN_TYPE_COMMENT,
        .sval = str,
        .pos = pos
    });
}

struct token* token_make_comment()
{
    if (nextc() != '/') return NULL;
    char c = nextc();
    if (c == '/') return token_make_one_line_comment();
    if (c == '*') return token_make_multiline_comment();
    pushc(c);
    pushc('/');
    return token_make_operator_or_string();
}