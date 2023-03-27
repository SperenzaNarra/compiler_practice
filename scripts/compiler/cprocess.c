#include "compiler.h"

#include "helpers/logger.h"

struct compile_process* compile_process_create(const char* filename, const char* out_filename, int flags)
{
    

    FILE* file = fopen(filename, "r");
    if (!file) return NULL; 

    FILE* out = NULL;
    if (out_filename)
    {
        out = fopen(out_filename, "w");
        if (!out) return NULL;
    }

    struct compile_process* process = malloc(sizeof(struct compile_process));
    *process = (struct compile_process)
    {
        .flags = flags,
        .cfile = (struct file_header){.fp = file, .path = filename},
        .ofile = out
    };

    return process;
}

void compile_process_free(struct compile_process* process)
{
    
    
    if (!process) return;
    fclose(process->cfile.fp);
    if (process->ofile) fclose(process->ofile);
    free(process);
}