#ifndef LEX_HELPER_H
#define LEX_HELPER_H

#include "helpers/logger.h"

#define NUMERIC_CASE \
    '0' ... '9'
#define STRING_CASE \
    '"'

// lexer.c
char nextc();
char peekc();
void pushc(char c);
struct pos* lex_process_pos();
struct vector* lexer_tokens();
struct token* lexer_last_token();

#define LEX_GETC_IF(buffer, c, exp)     \
    for (c = nextc(); exp; c = nextc()) \
    {                                   \
        buffer_write(buffer, c);        \
    }                                   \
    pushc(c);


// lex.c
struct token* read_next_token();
void char_not_found_error(struct logger* logger, struct pos pos, char c);

// lex_number.c
struct token* token_make_number();

// lex_string.c
struct token* token_make_string(char start_delim, char end_delim);

#endif