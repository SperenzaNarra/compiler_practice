#ifndef LEX_HELPER_H
#define LEX_HELPER_H

#include <stdbool.h>

#include "pos.h"
#include "helpers/logger.h"

#define NUMERIC_CASE \
    '0' ... '9'
#define STRING_CASE \
    '"'
#define OPERATOR_CASE_EXCLUDE_DIVISION  \
    '+':                                \
    case '-':                           \
    case '*':                           \
    case '>':                           \
    case '<':                           \
    case '^':                           \
    case '%':                           \
    case '!':                           \
    case '=':                           \
    case '~':                           \
    case '|':                           \
    case '&':                           \
    case '(':                           \
    case '[':                           \
    case ',':                           \
    case '.':                           \
    case '?'
#define SYMBOL_CASE \
    '{':            \
    case '}':       \
    case ':':       \
    case ';':       \
    case '#':       \
    case '\\':      \
    case ')':       \
    case ']'

#define LEX_GETC_IF(buffer, c, exp)     \
    for (c = nextc(); exp; c = nextc()) \
    {                                   \
        buffer_write(buffer, c);        \
    }                                   \
    pushc(c);
    
#define S_EQ(str1, str2) (str1 && str2 && (strcmp(str1, str2) == 0))

// lex.c
struct lex_process* lex_process;
void lex_error(struct logger* logger, struct pos pos, const char* msg, ...);

char nextc();
char peekc();
void pushc(char c);

struct pos* lex_process_pos();
void lex_process_set_pos(struct pos* pos);

struct vector* lexer_tokens();
struct token* lexer_last_token();

void lex_new_expression();
void lex_finish_expression();
bool lex_is_in_expression();

struct token* read_next_token();

// lex_number.c
struct token* token_make_number();

// lex_string.c
struct token* token_make_string(char start_delim, char end_delim);

// lex_operator.c
struct token* token_make_operator_or_string();

// lex_symbol.c
struct token* token_make_symbol();

// lex_identifier.c
struct token* token_make_identifier_or_keyword();

// lex_keyword.c
bool is_keyword(const char* str);
#endif