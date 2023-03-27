#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void log_message(
    const char* function, 
    const char* title, 
    const char* color,
    const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s[%s] %s: ", color, title, function);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "%s", ANSI_COLOR_RESET);
    va_end(args);
}


char* display_char(char c)
{
    char* str = malloc(8);
    memset(str, 0, 8);
    
    switch (c)
    {
    case '\0'   : strcpy(str, "\\0");   break;
    case '\001' : strcpy(str, "\\001"); break;
    case '\002' : strcpy(str, "\\002"); break;
    case '\003' : strcpy(str, "\\003"); break;
    case '\004' : strcpy(str, "\\004"); break;
    case '\005' : strcpy(str, "\\005"); break;
    case '\006' : strcpy(str, "\\006"); break;
    case '\007' : strcpy(str, "\\007"); break;
    case '\b'   : strcpy(str,"\\b");    break;
    case '\t'   : strcpy(str,"\\t");    break;
    case '\n'   : strcpy(str,"\\n");    break;
    case '\v'   : strcpy(str,"\\v");    break;
    case '\f'   : strcpy(str,"\\f");    break;
    case '\r'   : strcpy(str,"\\r");    break;
    case '\016' : strcpy(str, "\\016"); break;
    case '\017' : strcpy(str, "\\017"); break;
    case '\020' : strcpy(str, "\\020"); break;
    case '\021' : strcpy(str, "\\021"); break;
    case '\022' : strcpy(str, "\\022"); break;
    case '\023' : strcpy(str, "\\023"); break;
    case '\024' : strcpy(str, "\\024"); break;
    case '\025' : strcpy(str, "\\025"); break;
    case '\026' : strcpy(str, "\\026"); break;
    case '\027' : strcpy(str, "\\027"); break;
    case '\030' : strcpy(str, "\\030"); break;
    case '\031' : strcpy(str, "\\031"); break;
    case '\032' : strcpy(str, "\\032"); break;
    case '\033' : strcpy(str, "\\033"); break;
    case '\034' : strcpy(str, "\\034"); break;
    case '\035' : strcpy(str, "\\035"); break;
    case '\036' : strcpy(str, "\\036"); break;
    case '\037' : strcpy(str, "\\037"); break;
    case ' '    : strcpy(str, "\' \'"); break;
    case '\\'   : strcpy(str, "\\\\");  break;
    case '\''   : strcpy(str, "\\\'");  break;
    case '\"'   : strcpy(str, "\\\"");  break;
    case EOF    : strcpy(str, "EOF");   break;
    default     : str[0] = c;           break;
    }

    return str;
}