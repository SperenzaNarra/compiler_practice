#include "lexer.h"
#include "lexer/lex_helper.h"

#include <string.h>

#include "helpers/logger.h"

static char* read_number_str()
{
    log_debug("is called\n");
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
    log_debug("is called\n");
    char* str = read_number_str();
    unsigned long long num = atoll(str);
    free(str);
    return num;
}

int lexer_number_type(char c)
{
    log_debug("is called\n");
    switch (c)
    {
    case 'l':
    case 'L':
        return NUMBER_TYPE_LONG;
    case 'f':
    case 'F':
        return NUMBER_TYPE_FLOAT;
    default:
        return NUMBER_TYPE_NORMAL;
    }

}

struct token* token_make_number()
{
    log_debug("is called\n");
    struct pos pos = *lex_process_pos();
    unsigned long number = read_number();
    int number_type = lexer_number_type(peekc());

    if (number_type != NUMBER_TYPE_NORMAL) nextc();

    return token_create(&(struct token){
        .type = TOKEN_TYPE_NUMBER,
        .llnum = number,
        .pos = pos,
        .num = (struct token_number) {.type = number_type}
    });
}

static char lex_get_escaped_char(char c)
{
    log_debug("is called\n");
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
    log_debug("is called\n");
    struct pos pos = *lex_process_pos();

    char c = nextc();
    if (c != '\'') lex_error(pos, "read invalid character %c\n", c);

    c = nextc();
    if (c == '\\')
    {
        // escape case
        c = lex_get_escaped_char(nextc());
    }

    if (nextc() != '\'')
    {
        lex_error(pos, "You opened a quote ' but did not close it with a ' character\n");
    }

    return token_create(&(struct token){
        .type = TOKEN_TYPE_NUMBER,
        .cval = c,
        .pos = pos
    });
}

static bool validate_hexadecimal_str(const char* str)
{
    log_debug("is called\n");
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
    log_debug("is called\n");
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
    log_debug("is called\n");
    struct pos pos = *lex_process_pos();
    char* str = read_hexadecimal_str();

    if (!validate_hexadecimal_str(str))
    {
        lex_error(pos, "invalid hexdecimal value 0x%s\n", str);
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
    log_debug("is called\n");
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (str[i] != '0' || str[i] != '1') return false;
    }
    return true;
}

static struct token* token_make_binary()
{
    log_debug("is called\n");
    struct pos pos = *lex_process_pos();
    char* str = read_number_str();

    if (!validate_binary_str(str))
    {
        lex_error(pos, "invalid binary value 0b%s\n", str);
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
    log_debug("is called\n");
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
        lex_error(pos, "unable to read data %lldx\n", last_token->llnum);
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
        lex_error(pos, "invalid datatype 0%c\n", c);
    }

    return token;
}