#include "lexer.h"

#include "helpers/logger.h"

char lex_process_next_char(struct lex_process* process)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_next_char() is called\n");

    process->last_pos = process->pos;
    process->pos.col += 1;

    struct compile_process* compiler = process->compiler;
    char c = getc(compiler->cfile.fp);
    if (c == '\n')
    {
        process->pos.line += 1;
        process->pos.col = 1;
    }

    char* str = display_char(c);
    logger->debug(logger, "get char %s (line %d col %d)\n", str, process->pos.line, process->pos.col);
    free(str);

    return c;
}

char lex_process_peek_char(struct lex_process* process)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_peek_char() is called\n");

    struct compile_process* compiler = process->compiler;
    char c = getc(compiler->cfile.fp);
    ungetc(c, compiler->cfile.fp);

    char* str = display_char(c);
    logger->debug(logger, "get char %s (line %d col %d)\n", str, process->pos.line, process->pos.col);
    free(str);

    return c;
}

void lex_process_push_char(struct lex_process* process, char c)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_push_char() is called\n");

    char* str = display_char(c);
    logger->debug(logger, "get char %s\n", str);
    free(str);

    struct compile_process* compiler = process->compiler;
    process->pos = process->last_pos;
    ungetc(c, compiler->cfile.fp);
}

struct lex_process* lex_process_create(struct compile_process* compiler, void* private)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_create() is called\n");

    struct lex_process* process = malloc(sizeof(struct lex_process));
    if (!process) return NULL;

    *process = (struct lex_process)
    {
        .pos = (struct pos){.col = 1, .line = 1, .filename = compiler->cfile.path},
        .token_vec = vector_create(sizeof(struct token)),
        .compiler = compiler,
        .private = private,

        .next_char = lex_process_next_char,
        .peek_char = lex_process_peek_char,
        .push_char = lex_process_push_char
    };

    return process;
}


struct vector* lex_process_tokens(struct lex_process* process)
{
    return process->token_vec;
}


void* lex_process_private(struct lex_process* process)
{
    return process->private;
}

void lex_process_free(struct lex_process* process)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_free() is called\n");

    if (!process) return;
    vector_free(process->token_vec);
    free(process);
}

