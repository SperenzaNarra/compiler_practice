# 第二章 Token

Compiler (编译器）是一个将高级语言转化成机械语言的工具。

在过程中，他会经历六个过程

1. __词法分析 Lexical Analysis__
2. 语法分析 Syntactic Analysis
3. 语义分析 Semantic Analysis
4. 内部程序生成 Intermediate Code Generation
5. 代码优化 Code Optimization
6. 程序生成 Code Generation

而词法分析就是分析当前文字是属于哪一种类型，也不多，就8种

1. 标识符 identifier
2. 关键字 keyword
3. 运算符 operator
4. 符号 symbol
5. 数字 number
6. 字符串 string
7. 注释 comment
8. 新一行 new line

比如我输入 char* observer = "Seher" 我就会得到

- 关键字 char
- 运算符 *
- 标识符 observer
- 运算符 =
- 字符串 Seher

再详情的请去网上搜索，我就不多赘述了，下面是创建用于记录当前词法的工具 Token

<br>

## pos.h

表示当前位置的 struct，单独放一个头文件是因为有至少两个 header 需要引用它

```c
#ifndef POS_H
#define POS_H

struct pos
{
    int line;
    int col;
    const char* filename;
};

#endif //POS_H
```

## token.h

```c
#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pos.h"

enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE
};

struct token
{
    int type;
    int flags;
    
    union
    {
        char cval;
        const char* sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void* any;
    };

    bool whitespace;
    const char* between_brackets;
    struct pos pos;
};

struct token* token_create(struct token* _token);
#endif // TOKEN_H
```

就如上文所说，token总共有八类
- TOKEN_TYPE_IDENTIFIER 标识符 
- TOKEN_TYPE_KEYWORD 关键字
- TOKEN_TYPE_OPERATOR 运算符 
- TOKEN_TYPE_SYMBOL 符号 
- TOKEN_TYPE_NUMBER 数字 
- TOKEN_TYPE_STRING 字符串 
- TOKEN_TYPE_COMMENT 注释 
- TOKEN_TYPE_NEWLINE 新一行

在未来，每一个种类都会有一个单独的 lexer 的 c文件 处理

```c
const char* between_brackets;
```

是用于处理括号之间的情况

<br>

## 要完善的项目 (scripts/token.c)

```c
struct token* token_create(struct token* _token);
```
- 复制一个已存在的 token，返回一个新建立的 token

<br>

## 验证

你可以利用 logger 去显示制作结果，效果如下

```c
             token.c -   DEBUG - token_create() is called
             token.c -   DEBUG - read token
             token.c -   DEBUG -   type  : number
             token.c -   DEBUG -   value : 50
             token.c -   DEBUG -   whitespace : false
             token.c -   DEBUG -   between_brackets : (null)
             token.c -   DEBUG - read pos
             token.c -   DEBUG -   col  : 0
             token.c -   DEBUG -   line : 0
             token.c -   DEBUG -   filename : main.c
```