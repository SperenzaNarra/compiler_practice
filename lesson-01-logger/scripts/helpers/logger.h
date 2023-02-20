#ifndef LOGGER_H
#define LOGGER_H

#define DEBUG_FUNCTION_CALL
#define DEBUG_INFO

enum
{
    LOGGER_MESSAGE_LEVEL_INFO,
    LOGGER_MESSAGE_LEVEL_DEBUG,
    LOGGER_MESSAGE_LEVEL_WARNING,
    LOGGER_MESSAGE_LEVEL_ERROR
};

struct logger;
typedef void (*LOGGER_MESSAGE_FN)(struct logger* logger, const char* msg, ...);

struct logger
{
    const char* name;
    LOGGER_MESSAGE_FN info;
    LOGGER_MESSAGE_FN debug;
    LOGGER_MESSAGE_FN warning;
    LOGGER_MESSAGE_FN error;
};

struct logger* get_logger(const char* name, const char* fn_name);
void kill_logger(const char* name);
void kill_all_logger();
char* display_char(char c);

#endif // LOGGER_H