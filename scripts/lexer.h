#ifndef LEXER_H
#define LEXER_H

#include "pos.h"
#include "token.h"
#include "compiler.h"

#include "helpers/vector.h"
#include "helpers/buffer.h"

enum
{
    LEXICAL_ANALYSIS_ALL_OK,
    LEXICAL_ANALYSIS_INPUT_ERROR
};

struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process* process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process* process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process* process, char c);

struct lex_process_functions
{
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;
};

struct lex_process
{
    struct pos pos;
    struct pos last_pos;
    struct vector* token_vec;
    struct compile_process* compiler;

    int current_expression_count;
    struct buffer* parenthesis_buffer;

    // lex_process.c
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;

    // things that lexer wont understand
    void* private;
};


// lexer.c
int lex(struct lex_process* process);
char nextc();
char peekc();
void pushc(char c);
bool lex_is_in_expression();
void lex_process_free(struct lex_process* process);

// lex_process.c
struct lex_process* lex_process_create(struct compile_process* compiler, struct lex_process_functions* functions, void* private);
struct buffer* lex_parenthesis_buffer();

// lex_for_string.c
struct lex_process* token_build_for_string(struct compile_process* compiler, const char* str);

#endif // LEXER_H