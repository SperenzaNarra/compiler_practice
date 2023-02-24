#include "compiler.h"

#include "lexer.h"

#include "helpers/logger.h"

int compile_file(const char* filename, const char* out_filename, int flags)
{
    struct logger* logger = get_logger("compiler.c", "compile_file");

    // build an compile process
    struct compile_process* process = compile_process_create(filename, out_filename, flags);
    if (!process) return COMPILER_FILE_FAILED_WITH_ERRORS;

    // Perform Lexical Analysis
    struct lex_process *lex_process = lex_process_create(process, NULL);
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

    // FUTURE: Perform Parsing

    // FUTURE: Perform Code Generation

    // free everything
    lex_process_free(lex_process);
    compile_process_free(process);

    return COMPILER_FILE_COMPILED_OK;
}