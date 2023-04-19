#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "helpers/logger.h"

void read_history(struct history* history)
{
    log_debug("is called\n");

    log_debug("read flags\n");
    int flags = history->flags;
    if (flags & NODE_FLAG_INSIDE_EXPRESSION)
        log_info("  NODE_FLAG_INSIDE_EXPRESSION\n");
}

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
    log_debug("is called\n");
    struct history* new_history = malloc(sizeof(struct history));
    if (!new_history) return NULL;

    memcpy(new_history, history, sizeof(struct history));
    new_history->flags = flags;

    read_history(new_history);

    return new_history;
}