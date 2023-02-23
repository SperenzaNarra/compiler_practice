#ifndef LEX_HELPER_H
#define LEX_HELPER_H

#define NUMERIC_CASE \
    '0' ... '9'

// lexer.c
char nextc();
char peekc();
void pushc(char c);
struct pos* lex_process_pos();
struct token* lexer_last_token();

#define LEX_GETC_IF(buffer, c, exp)     \
    for (c = nextc(); exp; c = nextc()) \
    {                                   \
        buffer_write(buffer, c);        \
    }


// lex.c
struct token* read_next_token();

// lex_number.c
struct token* token_make_number();

#endif