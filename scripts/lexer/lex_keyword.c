#include "lexer.h"
#include "lexer/lex_helper.h"

#include <stdbool.h>

bool is_keyword(const char* str)
{
    return 
        S_EQ(str, "unsigned") ||
        S_EQ(str, "signed") ||
        S_EQ(str, "char") ||
        S_EQ(str, "int") ||
        S_EQ(str, "short") ||
        S_EQ(str, "long") ||
        S_EQ(str, "float") ||
        S_EQ(str, "double") ||
        S_EQ(str, "void") ||
        S_EQ(str, "struct") ||
        S_EQ(str, "union") ||
        S_EQ(str, "static") ||
        S_EQ(str, "inline") ||
        S_EQ(str, "__ignore_typecheck") ||
        S_EQ(str, "return") ||
        S_EQ(str, "include") ||
        S_EQ(str, "sizeof") ||
        S_EQ(str, "if") ||
        S_EQ(str, "else") ||
        S_EQ(str, "while") ||
        S_EQ(str, "for") ||
        S_EQ(str, "do") ||
        S_EQ(str, "break") ||
        S_EQ(str, "continue") ||
        S_EQ(str, "switch") ||
        S_EQ(str, "case") ||
        S_EQ(str, "default") ||
        S_EQ(str, "goto") ||
        S_EQ(str, "typedef") ||
        S_EQ(str, "const") ||
        S_EQ(str, "extern") ||
        S_EQ(str, "restrict");
}