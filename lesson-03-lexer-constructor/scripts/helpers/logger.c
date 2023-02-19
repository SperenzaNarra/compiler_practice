#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static inline const char* level_to_str(int level)
{
    switch (level)
    {
    case LOGGER_MESSAGE_LEVEL_INFO: 
        return "INFO";
    case LOGGER_MESSAGE_LEVEL_DEBUG: 
        return "DEBUG";
    case LOGGER_MESSAGE_LEVEL_WARNING: 
        return "WARNING";
    case LOGGER_MESSAGE_LEVEL_ERROR: 
        return "ERROR";
    default: 
        return "UNKNOWN";
    }
}

static inline void logger_message(const char* name, int level, const char* msg, va_list* args)
{
    fprintf(stderr, "%20s - %7s - ", name, level_to_str(level));
    vfprintf(stderr, msg, *args);
}

void logger_message_info(struct logger* logger, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    logger_message(logger->name, LOGGER_MESSAGE_LEVEL_INFO, msg, &args);
    va_end(args);
}

void logger_message_debug(struct logger* logger, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    logger_message(logger->name, LOGGER_MESSAGE_LEVEL_DEBUG, msg, &args);
    va_end(args);
}

void logger_message_warning(struct logger* logger, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    logger_message(logger->name, LOGGER_MESSAGE_LEVEL_WARNING, msg, &args);
    va_end(args);
}

void logger_message_error(struct logger* logger, const char* msg, ...)
{
    va_list args;
    va_start(args, msg);
    logger_message(logger->name, LOGGER_MESSAGE_LEVEL_ERROR, msg, &args);
    va_end(args);
}



struct logger** logger_list;
int logger_size = 8;

struct logger* create_logger(const char* name)
{
    if (!logger_list) logger_list = calloc(logger_size, sizeof(struct logger*));
    if (!logger_list) return NULL;
    
    // find empty
    int end;
    for (end = 0; end < logger_size && logger_list[end]; end++);

    // if full, doubling the logger_size
    if (logger_size == end)
    {
        logger_size = logger_size * 2;

        // If there is not enough storage to expand the block to the given size, 
        // the original block is unchanged and the realloc() function returns NULL.
        struct logger** tmp_logger_list = realloc(logger_list, sizeof(struct logger*) * logger_size);
        if (!tmp_logger_list) return NULL;
        logger_list = tmp_logger_list;
        
        // set every new pointers as NULL
        memset(logger_list+end, 0, (logger_size-end) * sizeof(struct logger*));
    }

    // setter
    struct logger* logger = malloc(sizeof(struct logger));
    if (!logger) return NULL;

    *logger = (struct logger)
    {
        .name = name,
        .info    = logger_message_info,
        .debug   = logger_message_debug,
        .warning = logger_message_warning,
        .error   = logger_message_error,
    };
    logger_list[end] = logger;

    return logger;
}

struct logger* get_logger(const char* name)
{
    if (!logger_list) return create_logger(name);

    // check history
    for (int i = 0; i < logger_size && logger_list[i]; i++)
    {
        if (strcmp(name, logger_list[i]->name) == 0) return logger_list[i];
    }

    // if not found
    return create_logger(name);
}


void kill_logger(const char* name)
{
    // ignore if nothing
    if (!logger_list) return;

    // find logger
    for (int index=0; index < logger_size; index++)
    {
        if (!logger_list[index]) continue;
        if (strcmp(name, logger_list[index]->name) == 0) 
        {
            free(logger_list[index]);
            logger_list[index] = NULL;
            return;
        }
    }
}

void kill_all_logger()
{
    if (!logger_list) return;
    for (int i = 0; i < logger_size; i++)
    {
        if (logger_list[i]) free(logger_list[i]);
    }
    free(logger_list);
    logger_size = 8;
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