#ifndef LOGGER_H
#define LOGGER_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char* display_char(char c);
void log_message(
    const char* function, 
    const char* title, 
    const char* color,
    const char* fmt, ...);

#ifdef INFO
#define log_info(fmt, ...) \
    log_message(__FUNCTION__, "INFO", ANSI_COLOR_RESET, fmt, ##__VA_ARGS__)
#else
#define log_info(fmt, ...)
#endif

#ifdef DEBUG
#define log_debug(fmt, ...) \
    log_message(__FUNCTION__, "DEBUG", ANSI_COLOR_GREEN, fmt, ##__VA_ARGS__)
#else
#define log_debug(fmt, ...)
#endif


#define log_warning(fmt, ...) \
    log_message(__FUNCTION__, "WARNING", ANSI_COLOR_YELLOW, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) \
    log_message(__FUNCTION__, "ERROR", ANSI_COLOR_RED, fmt, ##__VA_ARGS__)

#endif // LOGGER_H