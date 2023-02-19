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

## 要完善的项目 

### scripts\compiler\compiler.c

```c
int compile_file(const char* filename, const char* out_filename, int flags);
```

### scripts\compiler\cprocess.c

```c
struct compile_process* compile_process_create(const char* filename, const char* out_filename, int flags);
```

```c
void compile_process_free(struct compile_process* process);
```

### scripts\lexer\lexer.c

```c
int lex(struct lex_process* process);
```

### scripts\lexer\lex_process.c

```c
struct lex_process* lex_process_create(struct compile_process* compiler, void* private);
```

```c
struct vector* lex_process_tokens(struct lex_process* process);
```

```c
void* lex_process_private(struct lex_process* process);
```

```c
void lex_process_free(struct lex_process* process);
```


