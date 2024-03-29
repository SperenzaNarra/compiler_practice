#include "compiler.h"

#include "lexer.h"
#include "parser.h"
#include "node.h"

#include "helpers/vector.h"
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

    log_debug("LEXICAL ANALYSIS COMPLETE\n\n");

    // Perform Parsing

    process->node_vec = vector_create(sizeof(struct node*));
    process->node_tree_vec = vector_create(sizeof(struct node*));
    if (parse(process) != PARSE_ALL_OK)
    {
        lex_process_free(lex_process);
        if (process->node_vec) vector_free(process->node_vec);
        if (process->node_tree_vec) vector_free(process->node_tree_vec);
        compile_process_free(process);
        return COMPILER_FILE_FAILED_WITH_ERRORS;
    }


    // FUTURE: Perform Code Generation

    // free everything
    lex_process_free(lex_process);
    compile_process_free(process);

    return COMPILER_FILE_COMPILED_OK;
}