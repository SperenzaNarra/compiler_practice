#ifndef HISTORY_H
#define HISTORY_H

struct history
{
    int flags;
};

struct history* history_begin(int flags);
struct history* history_down(struct history* history, int flags);

#endif // HISTORY_H