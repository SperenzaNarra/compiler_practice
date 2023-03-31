#ifndef POS_H
#define POS_H

struct pos
{
    int line;
    int col;
    const char* filename;
};

void read_pos(struct pos* pos);

#endif //POS_H