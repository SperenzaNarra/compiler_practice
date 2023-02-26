#include "lexer.h"
#include "lexer/lex_helper.h"

#include <string.h>

#include "helpers/logger.h"

static inline const char* read_number_str(struct buffer* buffer)
{
    struct logger* logger = get_logger("lex_number", "read_number_str");

    char c;
    LEX_GETC_IF(buffer, c, (c >= '0' && c <= '9'));
    buffer_write(buffer, 0x00);

    return buffer_ptr(buffer);
}

static inline unsigned long long read_number()
{
    struct logger* logger = get_logger("lex_number", "read_number");

    struct buffer* buffer = buffer_create();
    const char* s = read_number_str(buffer);
    unsigned long long num = atoll(s);
    buffer_free(buffer);
    return num;
}

struct token* token_make_number()
{
    struct logger* logger = get_logger("lex_number", "token_make_number");

    struct pos pos = *lex_process_pos();
    return token_create(&(struct token){
        .type = TOKEN_TYPE_NUMBER,
        .llnum = read_number(),
        .pos = pos
    });
}