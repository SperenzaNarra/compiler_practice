#ifndef HISTORY_H
#define HISTORY_H

#include <stdbool.h>

enum
{
    HISTORY_LAST_TYPE_NONE,
    HISTORY_LAST_TYPE_VALUE,
    HISTORY_LAST_TYPE_OPERATOR,
    HISTORY_LAST_TYPE_END
};

struct history
{
    int flags;
    int last_type;
};

struct history* history_begin(int flags);
struct history* history_down(struct history* history, int flags);
void read_history(struct history* history);

#endif // HISTORY_H