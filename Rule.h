#include <stdio.h>
#ifndef Rule__
#define Rule__

struct Rule
{
    int level = 0;
    int score = 0;
    int speed = 0;
    int bonus = 0;
};

struct Rules
{
    Rule *rules = 0;
    int size = 0;
};


bool Read(FILE *fp, Rules *rules);
bool Add(Rules *rules, const Rule *rule);
void Free(Rules *rules);
void Print(const Rules *rules);

int GetLevel(const Rules *rules, int score);
int GetSpeed(const Rules *rules, int LV);
int GetBonus(const Rules *rules, int LV);

int UpdateScore(const Rules *rules, int level, int line);

#endif /* Rule__ */
