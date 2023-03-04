#include "lexer.h"
#include "lexer/lex_helper.h"

#include <string.h>

#include "helpers/logger.h"

static char* read_number_str()
{
    struct logger* logger = get_logger("lex_number", "read_number_str");
    struct buffer* buffer = buffer_create();
    char c;
    LEX_GETC_IF(buffer, c, (c >= '0' && c <= '9'));
    buffer_write(buffer, 0x00);
    char* str = strdup(buffer_ptr(buffer));
    buffer_free(buffer);

    return str;
}

static unsigned long long read_number()
{
    struct logger* logger = get_logger("lex_number", "read_number");
    
    char* str = read_number_str();
    unsigned long long num = atoll(str);
    free(str);
    return num;
}

struct token* token_make_number()
{
    struct logger* logger = get_logger("lex_number", "token_make_number");

    struct pos pos = *lex_process_pos();
    return token_create(&(struct token){
        .type = TOKEN_TYPE_NUMBER,
        .llnum = read_number(),
        .pos = pos
    });
}

static char lex_get_escaped_char(char c)
{
    struct logger* logger = get_logger("lex_number", "lex_get_escaped_char");
    char out;
    switch (c)
    {
    case 'n':
        out = '\n';
        break;
    case 't':
        out = '\t';
        break;
    default:
        out = c;
        break;
    }
    return out;
}

struct token* token_make_quote()
{
    struct logger* logger = get_logger("lex_number", "token_make_quote");
    struct pos pos = *lex_process_pos();

    char c = nextc();
    if (c != '\'') lex_error(logger, pos, "read invalid character %c\n", c);

    c = nextc();
    if (c == '\\')
    {
        // escape case
        c = lex_get_escaped_char(nextc());
    }

    if (nextc() != '\'')
    {
        lex_error(logger, pos, "You opened a quote ' but did not close it with a ' character\n");
    }

    return token_create(&(struct token){
        .type = TOKEN_TYPE_NUMBER,
        .cval = c,
        .pos = pos
    });
}

static bool validate_hexadecimal_str(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] >= '0' && str[i] <= '9') continue;
        if (str[i] >= 'a' && str[i] <= 'f') continue;
        if (str[i] >= 'A' && str[i] <= 'F') continue;
        return false;
    }
    return true;
}

static char* read_hexadecimal_str()
{
    struct buffer* buffer = buffer_create();
    char c;
    LEX_GETC_IF(buffer, c, 
        c >= '0' && c <= '9' ||
        c >= 'a' && c <= 'z' ||
        c >= 'A' && c <= 'Z'
    );
    buffer_write(buffer, 0x00);
    char* str = strdup(buffer_ptr(buffer));
    buffer_free(buffer);
    return str;
}

static struct token* token_make_hexadecimal()
{
    struct logger* logger = get_logger("lex_number", "token_make_hexadecimal");
    struct pos pos = *lex_process_pos();
    char* str = read_hexadecimal_str();

    if (!validate_hexadecimal_str(str))
    {
        lex_error(logger, pos, "invalid hexdecimal value 0x%s\n", str);
    }

    unsigned long number = strtol(str, 0, 16);
    free(str);
    return token_create(&(struct token){
        .type = TOKEN_TYPE_NUMBER,
        .llnum = number,
        .pos = pos
    });
}

static bool validate_binary_str(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] != '0' || str[i] != '1') return false;
    }
    return true;
}

static struct token* token_make_binary()
{
    struct logger* logger = get_logger("lex_number", "token_make_binary");
    struct pos pos = *lex_process_pos();
    char* str = read_number_str();

    if (!validate_binary_str(str))
    {
        lex_error(logger, pos, "invalid binary value 0b%s\n", str);
    }

    unsigned long number = strtol(str, 0, 2);
    return token_create(&(struct token){
        .type = number,
        .llnum = number,
        .pos = pos
    });  
}

struct token* token_make_special_number()
{
    struct logger* logger = get_logger("lex_number", "token_make_special_number");
    struct pos pos = *lex_process_pos();

    struct token* token = NULL;
    struct token* last_token = lexer_last_token();

    if (
        !last_token ||  // no last token
        last_token->whitespace || // last token with whitespace
        last_token->type != TOKEN_TYPE_NUMBER // last token is not a number
    ) return token_make_identifier_or_keyword();

    // assume it is number
    if (last_token->llnum != 0x00)
    {
        lex_error(logger, pos, "unable to read data %lldx\n", last_token->llnum);
    }

    lexer_pop_token();

    char c = nextc();
    switch (c)
    {
    case 'x':
    case 'X':
        // 0x00 hexadecimal case
        token = token_make_hexadecimal();
        break;
    case 'b':
    case 'B':
        // 0b00 binary case
        token = token_make_binary();
        break;
    default:
        lex_error(logger, pos, "invalid datatype 0%c\n", c);
    }

    return token;
}