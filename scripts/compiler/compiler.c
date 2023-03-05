#include "compiler.h"

#include "lexer.h"

#include "helpers/logger.h"


char lex_process_next_char(struct lex_process* process)
{
    struct compile_process* compiler = process->compiler;
    char c = getc(compiler->cfile.fp);

    return c;
}

char lex_process_peek_char(struct lex_process* process)
{
    struct compile_process* compiler = process->compiler;
    char c = getc(compiler->cfile.fp);
    ungetc(c, compiler->cfile.fp);

    return c;
}

void lex_process_push_char(struct lex_process* process, char c)
{
    struct compile_process* compiler = process->compiler;
    ungetc(c, compiler->cfile.fp);
}


struct lex_process_functions compiler_lex_functions = (struct lex_process_functions)
{
    .next_char = lex_process_next_char,
    .peek_char = lex_process_peek_char,
    .push_char = lex_process_push_char
};

int compile_file(const char* filename, const char* out_filename, int flags)
{
    struct logger* logger = get_logger("compiler.c", "compile_file");

    // build an compile process
    struct compile_process* process = compile_process_create(filename, out_filename, flags);
    if (!process) return COMPILER_FILE_FAILED_WITH_ERRORS;

    // Perform Lexical Analysis
    struct lex_process *lex_process = lex_process_create(process, &compiler_lex_functions, NULL);
    if (!lex_process) 
    {
        compile_process_free(process);
        return COMPILER_FILE_FAILED_WITH_ERRORS;
    }

    if (lex(lex_process) != LEXICAL_ANALYSIS_ALL_OK)
    {
        lex_process_free(lex_process);
        compile_process_free(process);
        return COMPILER_FILE_FAILED_WITH_ERRORS;
    }
    process->token_vec = lex_process->token_vec;

    // FUTURE: Perform Parsing

    // FUTURE: Perform Code Generation

    // free everything
    lex_process_free(lex_process);
    compile_process_free(process);

    return COMPILER_FILE_COMPILED_OK;
}