#ifndef HISTORY_H
#define HISTORY_H

struct history
{
    int flags;
};

struct history* history_begin(int flags);
struct history* history_down(struct history* history, int flags);
void read_history(struct history* history);

#endif // HISTORY_H