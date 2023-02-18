# 第一章 Logger
工欲善其事，必先利其器。在我们开始制作编程之前，我们要先制作一个简易的 Debug 工具

本篇的要求是根据logger.h的描述完善logger.c

## logger.h

```c
#ifndef LOGGER_H
#define LOGGER_H

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

struct logger* get_logger(const char* name);
void kill_logger(const char* name);
void kill_all_logger();
char* display_char(char c);

#endif // LOGGER_H
```

以上是我的logger蓝图，设计上我参考了python中的logging模块
<br>
### 报告等级
我在这设计了四个报告等级，用于应对各种不同的状况
- 通常级：LOGGER_MESSAGE_LEVEL_INFO
- 诊断级：LOGGER_MESSAGE_LEVEL_DEBUG
- 警告级：LOGGER_MESSAGE_LEVEL_WARNING
- 致命级：LOGGER_MESSAGE_LEVEL_ERROR
为这四个报告等级，我定义了一个类别

```c
typedef void (*LOGGER_MESSAGE_FN)(struct logger* logger, const char* msg, ...);
```

这行代表类别 LOGGER_MESSAGE_FN 是一个函数指针

它不会返回任何东西，它需要struct logger指针，一个String数据，以及各种其他变量
<br>
然后在 struct logger 里面添加四个函数通道

```c
LOGGER_MESSAGE_FN info;
LOGGER_MESSAGE_FN debug;
LOGGER_MESSAGE_FN warning;
LOGGER_MESSAGE_FN error;
```

这样就可以实现

```c
logger->info(logger, "hello world!");
```
<br>
### 通用函数

struct logger* get_logger(const char* name);
- 该函数会返回一个struct logger的指针
- 引用该函数时，系统会寻找是否已经存在同名的 logger，如果有就返回该 logger，反之创建新的 logger

void kill_logger(const char* name);
- 删除指定的 logger

void kill_all_logger();
- 删除全部的 logger

char* display_char(char c);
- 将一个字符转换成更加易读的文字

<br>
## logger.c
在创建各种函数之前，我们需要引用一些模块以方便后续的编程
```c
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
```

### 报告等级
首先我们需要制作一个简易的显示报告等级的函数，代码我就不赘述了
```c
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
```
<br>
再来我们需要制作一个简易的报告函数
```c
static inline void logger_message(const char* name, int level, const char* msg, va_list* args)
{
    fprintf(stderr, "%20s - %7s - ", name, level_to_str(level));
    vfprintf(stderr, msg, *args);
}
```

int fprintf(FILE *stream, const char *format, ...);
- 该函数用于输出数据到指定的流文件中，基本上什么文件都可以。

extern FILE *stderr;
- stderr 是系统预设用于输出错误的流文件。

/* unspecified */ va_list;
- va_list 是在C语言中解决变参问题的一组宏，在这情况我们能假设这里面已经存储了无限量的变量。

int vfprintf(FILE *stream, const char *format, va_list ap);
- 针对于 va_list 制作的输出函数，会结合 format 和 ap 中的数据生成格式化数据并将其输出到指定的流文件中。

<br>
然后写出四个通用函数
```c
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
```

这四个函数都是同样的事情：读取潜在的无限变量，并引用 logger_message 函数

### create_logger
考虑到不同的函数会引用同一个logger，所以我决定让logger以全局变量的形式出现在代码里，这样可能会有内存短缺的风险，但如果要彻底的解决该问题就很花时间，且复杂。所以本系列不会太考虑这种事。

首先我们创建一个两层指针 logger_list 用于存储新生成的 logger，初始大小设定为8；
```c
struct logger** logger_list;
int logger_size = 8;

struct logger* create_logger(const char* name)
{
    if (!logger_list) logger_list = calloc(logger_size, sizeof(struct logger*));
    if (!logger_list) return NULL;
```
然后寻找最初的空闲空间
```c
    // find empty
    int end;
    for (end = 0; end < logger_size && logger_list[end]; end++);
```
如果已经没有空间了，就加大空间，并将新增的空间设为NULL
```c
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
```
如果一切状态如常，就建立一个新的 logger
```c
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
```
<br>
### get_logger
如果没有 logger 那就创建 logger
```c
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
```

<br>
### kill_logger
删除特定 logger
```c
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
```
<br>
### kill_all_logger
清空 logger
```c
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
```
<br>
### display_char
> If a switch contains more than five items, it's implemented using a lookup table or a hash list. This means that all items get the same access time, compared to a list of if:s where the last item takes much more time to reach as it has to evaluate every previous condition first.

虽然现在的编译器已经进化到了能够把烂代码优化到神代码，但我几个理由不选择用 if-else

1. 判断具备离散性：是1就是1，是2就是2，不存在1和2之间的情况
2. 显而易见的易读性
3. switch方法只会读取一次 c 变量，在大量的判断情况下速度会优于 if-else

```c
char* display_char(char c)
{
    char* str = calloc(8, 1);
    
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
```
<br>
## 验证
把 main.c 修改
```c
#include <stdio.h>
#include "helpers/logger.h"

int main(int argc, char *argv[])
{
    struct logger* logger = get_logger("main.c");
    logger->info(logger, "%s test\n", "info");
    logger->debug(logger, "%s test\n", "debug");
    logger->warning(logger, "%s test\n", "warning");
    logger->error(logger, "%s test\n", "error");
    kill_all_logger();

    if (argc == 1) return 0;

    char* arg = argv[1];
    printf("%s\n", arg);
    return 0;
}
```
得到下列结果就是成功
```c
              main.c -    INFO - info test
              main.c -   DEBUG - debug test
              main.c - WARNING - warning test
              main.c -   ERROR - error test
```