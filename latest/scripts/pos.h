#ifndef POS_H
#define POS_H

struct pos
{
    int line;
    int col;
    const char* filename;
};

#endif //POS_H