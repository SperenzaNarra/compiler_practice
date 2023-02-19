#include <stdio.h>
#include "token.h"

int main(int argc, char *argv[])
{
    struct Token *token = token_create(&(struct token){
        .type = TOKEN_TYPE_NUMBER,
        .inum = 50,
        .pos = (struct pos){.filename = "main.c"}
    });
    return 0;
}