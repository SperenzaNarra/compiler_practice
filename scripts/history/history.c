#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct history* history_begin(int flags)
{
    struct history* history = malloc(sizeof(struct history));
    if (!history) return NULL;
    *history = (struct history){
        .flags = flags,
    };
    return history;
}

struct history* history_down(struct history* history, int flags)
{
    struct history* new_history = malloc(sizeof(struct history));
    if (!new_history) return NULL;

    memcpy(new_history, history, sizeof(struct history));
    new_history->flags = flags;

    return new_history;
}