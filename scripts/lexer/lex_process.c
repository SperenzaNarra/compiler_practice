#include "lexer.h"
#include "lexer/lex_helper.h"

#include "helpers/logger.h"

struct lex_process* lex_process_create(struct compile_process* compiler, struct lex_process_functions* functions, void* private)
{
    struct logger* logger = get_logger("lex_process.c", "lex_process_create");

    struct lex_process* process = malloc(sizeof(struct lex_process));
    if (!process) return NULL;

    *process = (struct lex_process)
    {
        .pos = (struct pos){.col = 1, .line = 1, .filename = compiler->cfile.path},
        .token_vec = vector_create(sizeof(struct token)),
        .compiler = compiler,
        .private = private,

        .next_char = functions->next_char,
        .peek_char = functions->peek_char,
        .push_char = functions->push_char
    };

    return process;
}


struct vector* lex_process_tokens()
{
    return lex_process->token_vec;
}


void* lex_process_private()
{
    return lex_process->private;
}

struct pos* lex_process_pos()
{
    return &lex_process->pos;
}

void lex_process_set_pos(struct pos* pos)
{
    lex_process->pos = *pos;
}

struct token* lexer_last_token()
{
    return vector_back_or_null(lex_process->token_vec);
}

struct buffer* lex_parenthesis_buffer()
{
    return lex_process->parenthesis_buffer;
}

void lex_process_free(struct lex_process* process)
{
    struct logger* logger = get_logger("lex_process.c", "lex_process_free");

    if (!process) return;
    vector_free(process->token_vec);
    free(process);
}