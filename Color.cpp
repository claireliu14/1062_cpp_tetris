#include "Color.h"
#include <stdlib.h>
#include <stdio.h>

constexpr Color AppColor[SYSCOLORCOUNT] =
{
    {.86, .86, .86}, // gray
    {1.0, 1.0, 1.0}  // white
};

bool Add(Colors *colors, const Color *c)
{
    Color *p = (Color*) malloc(sizeof(Color)*(colors->size + 1));
    if(p==0) return false;

    for(int i=0; i<colors->size; i++)
    {
        p[i] = colors->colors[i];
    }
    p[colors->size] = *c;
    free(colors->colors);

    colors->colors = p;
    colors->size += 1;
    return true;
}

void ReadAppColor(Colors *colors)
{
    for(int i=0; i<SYSCOLORCOUNT; i++)
    {
        Color c;
        c.r = AppColor[i].r;
        c.g = AppColor[i].g;
        c.b = AppColor[i].b;

        if(!Add(colors, &c)) break;
    }
}

bool Read(FILE *fp, Colors *colors)
{
    if(!fp) return false;
    ReadAppColor(colors);

    Color c;
    while(fscanf(fp, "%f, %f, %f", &c.r, &c.g, &c.b)==3)
    {
        if(!Add(colors, &c)) break;
    }
    return true;
}

void Free(Colors *colors)
{
    if(colors->size) free(colors->colors);
    colors->colors = 0;
    colors->size = 0;
}

Color GetColor(const Colors *colors, int colorid)
{
    return colors->colors[colorid];
}

void Print(const Colors *colors)
{
    for(int i=0; i<colors->size; i++)
    {
        printf("%.2f %.2f %.2f\n", colors->colors[i].r, colors->colors[i].g, colors->colors[i].b);
    }
    printf("Color's size: %d\n", colors->size);
}
