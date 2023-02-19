# compiler.c

```c
#include "compiler.h"

#include "lexer.h"

#include "helpers/logger.h"
```

## compile_file

```c
int compile_file(const char* filename, const char* out_filename, int flags)
{
    struct logger* logger = get_logger("compiler.c");
    logger->debug(logger, "compile_file() is called\n");
```

创建构造体 compile_process

```c
    struct compile_process* process = compile_process_create(filename, out_filename, flags);
    if (!process) return COMPILER_FILE_FAILED_WITH_ERRORS;
```

创建构造体 lex_process

```c
    struct lex_process *lex_process = lex_process_create(process, NULL);
    if (!lex_process) 
    {
        compile_process_free(process);
        return COMPILER_FILE_FAILED_WITH_ERRORS;
    }
```

解析数据，将其转换成一组 token

```c
    if (lex(lex_process) != LEXICAL_ANALYSIS_ALL_OK)
    {
        lex_process_free(lex_process);
        compile_process_free(process);
        return COMPILER_FILE_FAILED_WITH_ERRORS;
    }
```

未来规划以及收尾

```c
    // FUTURE: Perform Parsing

    // FUTURE: Perform Code Generation

    // free everything
    lex_process_free(lex_process);
    compile_process_free(process);

    return COMPILER_FILE_COMPILED_OK;
}
```

# cprocess.c

```c
#include "compiler.h"

#include "helpers/logger.h"
```

## compile_process_create

```c
struct compile_process* compile_process_create(const char* filename, const char* out_filename, int flags)
{
    struct logger* logger = get_logger("cprocess.c");
    logger->debug(logger, "compile_process_create() is called\n");
```

开启输入文件

```c
    FILE* file = fopen(filename, "r");
    if (!file) return NULL; 
```

开启输出文件

```c
    FILE* out = NULL;
    if (out_filename)
    {
        out = fopen(out_filename, "w");
        if (!out) return NULL;
    }
```

如果一切如常，创建构造体 compile_process

```c
    struct compile_process* process = malloc(sizeof(struct compile_process));
    *process = (struct compile_process)
    {
        .flags = flags,
        .cfile = (struct file_header){.fp = file, .path = filename},
        .ofile = out
    };

    return process;
}
```

## compile_process_free

```c
void compile_process_free(struct compile_process* process)
{
    struct logger* logger = get_logger("cprocess.c");
    logger->debug(logger, "compile_process_free() is called\n");
    
    if (!process) return;
    fclose(process->cfile.fp);
    if (process->ofile) fclose(process->ofile);
    free(process);
}
```

原则上应该避免 fclose 一个 NULL，所以我添加了一个判断

# lex_process.c

```c
#include "lexer.h"

#include "helpers/logger.h"
```

## lex_process_next_char

```c
char lex_process_next_char(struct lex_process* process)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_next_char() is called\n");
```

更新位置

```c
    process->last_pos = process->pos;
    process->pos.col += 1;
```

读取字符，如果为下一行，再更新位置

```c
    struct compile_process* compiler = process->compiler;
    char c = getc(compiler->cfile.fp);
    if (c == '\n')
    {
        process->pos.line += 1;
        process->pos.col = 1;
    }
```

输出

```c
    char* str = display_char(c);
    logger->debug(logger, "get char %s with line %d col %d\n", str, process->pos.line, process->pos.col);
    free(str);

    return c;
}
```

## lex_process_peek_char

```c
char lex_process_peek_char(struct lex_process* process)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_peek_char() is called\n");
```

读取下一字符，留在原地

```c
    struct compile_process* compiler = process->compiler;
    char c = getc(compiler->cfile.fp);
    ungetc(c, compiler->cfile.fp);
```

输出

```c
    char* str = display_char(c);
    logger->debug(logger, "get char %s with line %d col %d\n", str, process->pos.line, process->pos.col);
    free(str);

    return c;
}
```

## lex_process_push_char

```c
void lex_process_push_char(struct lex_process* process, char c)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_push_char() is called\n");

    char* str = display_char(c);
    logger->debug(logger, "get char %s\n", str);
    free(str);
```

将字符 c 替换当前字符，并退一位

```c
    struct compile_process* compiler = process->compiler;
    process->pos = process->last_pos;
    ungetc(c, compiler->cfile.fp);
}
```

这里我的想法是，假如你要退很多位，那么就应该实现记录好最初的位置，然后最后重置位置，平时退一位就够了。

## lex_process_create

```c
struct lex_process* lex_process_create(struct compile_process* compiler, void* private)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_create() is called\n");

```

创建构造体 lex_process，预设初始点为（1，1）

```c
    struct lex_process* process = malloc(sizeof(struct lex_process));
    if (!process) return NULL;

    *process = (struct lex_process)
    {
        .pos = (struct pos){.col = 1, .line = 1, .filename = compiler->cfile.path},
        .token_vec = vector_create(sizeof(struct token)),
        .compiler = compiler,
        .private = private,

        .next_char = lex_process_next_char,
        .peek_char = lex_process_peek_char,
        .push_char = lex_process_push_char
    };

    return process;
}
```

getter

```c
struct vector* lex_process_tokens(struct lex_process* process)
{
    return process->token_vec;
}


void* lex_process_private(struct lex_process* process)
{
    return process->private;
}
```

## lex_process_free
删除构造体 lex_process

```c
void lex_process_free(struct lex_process* process)
{
    struct logger* logger = get_logger("lex_process.c");
    logger->debug(logger, "lex_process_free() is called\n");

    if (!process) return;
    vector_free(process->token_vec);
    free(process);
}
```

