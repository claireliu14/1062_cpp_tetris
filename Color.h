#include <stdio.h>

#ifndef Color__
#define Color__

struct Color{
    float r;
    float g;
    float b;
};

struct Colors{
    Color *colors = nullptr;
    int size = 0;
};

enum ColorName { GRAY, WHITE, SYSCOLORCOUNT };

bool Add(Colors *colors, const Color *c);
bool Read(FILE *fp, Colors *colors);
void ReadAppColor(Colors *colors);
void Free(Colors *colors);
void Print(const Colors *colors);
Color GetColor(const Colors *colors, int colorid);

#endif /* Color__ */
