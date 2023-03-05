#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>

#include "helpers/vector.h"

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

    struct vector* token_vec;
};

// compiler.c
int compile_file(const char* filename, const char* out_filename, int flags);

// cprocess.c
struct compile_process* compile_process_create(const char* filename, const char* out_filename, int flags);
void compile_process_free(struct compile_process* process);

#endif // COMPILER_H