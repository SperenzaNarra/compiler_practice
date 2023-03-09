#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pos.h"

enum
{
    NUMBER_TYPE_NORMAL,
    NUMBER_TYPE_LONG,
    NUMBER_TYPE_FLOAT,
    NUMBER_TYPE_DOUBLE,
};

struct token_number
{
    int type;
};

enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE
};

struct token
{
    int type;
    int flags;
    
    union
    {
        char cval;
        const char* sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void* any;
    };

    bool whitespace;
    char* between_brackets;
    struct pos pos;

    struct token_number num;
};

struct token* token_create(struct token* _token);
bool token_is_keyword(struct token* token, const char* keyword);

// DEBUG
void read_token(struct token* token);
#endif // TOKEN_H