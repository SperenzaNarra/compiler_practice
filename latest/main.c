#include <stdio.h>
#include "token.h"

int main(int argc, char *argv[])
{
    struct token *token = token_create(&(struct token){
        .type = TOKEN_TYPE_NUMBER,
        .inum = 50,
        .pos  = (struct pos) {.filename = "main.c"}
    });

    if (argc == 1) return 0;

    char* arg = argv[1];
    printf("%s\n", arg);
    return 0;
}