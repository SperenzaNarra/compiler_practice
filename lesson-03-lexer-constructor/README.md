# 第三章 Lexer Constructor

## compiler.h

```c
#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>

enum
{
    COMPILER_FILE_COMPILED_OK,
    COMPILER_FILE_FAILED_WITH_ERRORS
};

struct file_header
{
    FILE* fp;
    const char* path;
};

struct compile_process
{
    int flags;
    struct file_header cfile;
    FILE* ofile;
};

// compiler.c
int compile_file(const char* filename, const char* out_filename, int flags);

// cprocess.c
struct compile_process* compile_process_create(const char* filename, const char* out_filename, int flags);
void compile_process_free(struct compile_process* process);

#endif // COMPILER_H
```

### 构造体
如你们所见，我建立了两个构造体

- file_header 用于记录文档信息
- compile_process 用于记录编译当前状况

其中 compile_process 的命名原因为

- cfile: compile file 编译目标文件
- cfile: compile file 编译目标文件

### 信号

1. COMPILER_FILE_COMPILED_OK
2. COMPILER_FILE_FAILED_WITH_ERRORS

## lexer.h

```c
#ifndef LEXER_H
#define LEXER_H

#include "pos.h"
#include "token.h"
#include "compiler.h"

#include "helpers/vector.h"
#include "helpers/buffer.h"

enum
{
    LEXICAL_ANALYSIS_ALL_OK,
    LEXICAL_ANALYSIS_INPUT_ERROR
};

struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process* process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process* process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process* process, char c);

struct lex_process
{
    struct pos pos;
    struct pos last_pos;
    struct vector* token_vec;
    struct compile_process* compiler;

    int current_expression_count;
    struct buffer* parenthesis_buffer;

    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;

    // things that lexer wont understand
    void* private;
};

// lexer.c
int lex(struct lex_process* process);

// lex_process.c
struct lex_process* lex_process_create(struct compile_process* compiler, void* private);
struct vector* lex_process_tokens(struct lex_process* process);
void* lex_process_private(struct lex_process* process);
void lex_process_free(struct lex_process* process);

#endif // LEXER_H
```

### 构造体

用于记录 lexical analysis 的 lex_process
- pos : 当前字符位置
- last_pos ：上一个字符的位置
- token_vec ：用于储存生成的 token
- compiler ： 指向 compile_process 的指针

这四个是简单的记录信息

- current_expression_count ：记录当前处于几层括号夹层内
- parenthesis_buffer ： 用于记录括号之间的字符串

这两个变量用于处理出现括号的情况

- next_char ：读取下一字符，进一位
- peek_char ：读取下一字符，留在原地
- push_char ：将字符 c 替换当前字符，并退一位

这三个函数指针是用于基本的文件读取作业

- private ：用于储存 lexer 都不懂的东西

有些时候你需要放额外的东西在构造体里


## 要完善的项目 

### scripts\compiler\compiler.c

```c
int compile_file(const char* filename, const char* out_filename, int flags);
```

- 全编译程序的起点，也是整个编译流程大纲

### scripts\compiler\cprocess.c

```c
struct compile_process* compile_process_create(const char* filename, const char* out_filename, int flags);
```

- 创建构造体 compile_process

```c
void compile_process_free(struct compile_process* process);
```

- 删除构造体 compile_process

### scripts\lexer\lex_process.c

```c
struct lex_process* lex_process_create(struct compile_process* compiler, void* private);
```

- 创建构造体 lex_process

```c
struct vector* lex_process_tokens(struct lex_process* process);
```

- 输出构造体 lex_process 里面所有的 token

```c
void* lex_process_private(struct lex_process* process);
```

- 输出不明数据

```c
void lex_process_free(struct lex_process* process);
```

- 删除构造体 lex_process
