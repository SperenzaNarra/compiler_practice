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

然后在 struct logger 里面添加四个function pointer

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

## 要完善的项目 (scripts/helpers/logger.c)

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

## 验证

得到下列结果就是成功
```c
              main.c -    INFO - info test
              main.c -   DEBUG - debug test
              main.c - WARNING - warning test
              main.c -   ERROR - error test
```