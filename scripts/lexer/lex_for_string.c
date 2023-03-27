#include "lexer.h"
#include "lexer/lex_helper.h"

#include "helpers/buffer.h"

char lex_string_buffer_next_char(struct lex_process* process)
{
    struct buffer* buffer = lex_process_private(process);
    return buffer_read(buffer);
}

char lex_string_buffer_peek_char(struct lex_process* process)
{
    struct buffer* buffer = lex_process_private(process);
    return buffer_peek(buffer);
}

void lex_string_buffer_push_char(struct lex_process* process, char c)
{
    struct buffer* buffer = lex_process_private(process);
    buffer_write(buffer, c);
}

struct lex_process_functions lex_for_string_functions = (struct lex_process_functions){
    .next_char = lex_string_buffer_next_char,
    .peek_char = lex_string_buffer_peek_char,
    .push_char = lex_string_buffer_push_char
};

struct lex_process* token_build_for_string(struct compile_process* compiler, const char* str)
{
    
    struct buffer* buffer = buffer_create();
    buffer_printf(buffer, str);

    struct lex_process* lex_process = lex_process_create(compiler, &lex_for_string_functions, buffer);
    if (!lex_process)
    {
        buffer_free(buffer);
    }
    if (lex(lex_process) != LEXICAL_ANALYSIS_ALL_OK)
    {
        lex_process_free(lex_process);
        buffer_free(buffer);
        return NULL;
    }

}