#include "Rule.h"
#include <stdlib.h>

bool Read(FILE *fp, Rules *rules)
{
    if(!fp) return false;

    Rule rule;
    while(fscanf(fp, "%d %d %d %d", &rule.level, &rule.speed, &rule.score, &rule.bonus)==4)
    {
        if(!Add(rules, &rule)) break;
    }
    return true;
}

bool Add(Rules *rules, const Rule *rule)
{
    Rule *p = (Rule *)malloc(sizeof(Rule)*(rules->size+1));
    if(p==0) return false;

    for(int i=0; i<rules->size; i++)
    {
        p[i] = rules->rules[i];
    }
    p[rules->size] = *rule;
    free(rules->rules);
    rules->rules = p;
    rules->size += 1;
    return true;
}

int UpdateScore(const Rules *rules, int level, int line)
{
    return line * rules->rules[level-1].bonus;
}

void Print(const Rules *rules)
{
    printf("%3s%7s%7s%7s\n", "LV", "Speed", "Score", "Bonus");
    for(int i=0; i<rules->size; i++)
    {
        printf("%3d%7d%7d%7d\n", rules->rules[i].level, rules->rules[i].speed,
               rules->rules[i].score, rules->rules[i].bonus);
    }
}

void Free(Rules *rules)
{
    if(rules->size) free(rules->rules);
    rules->rules = 0;
    rules->size = 0;
}


int GetLevel(const Rules *rules, int score)
{
    for(int i=0; i<rules->size; i++)
    {
        if(rules->rules[i].score > score) return rules->rules[i].level;
    }
    return rules->rules[rules->size-1].level;
}

int GetSpeed(const Rules *rules, int LV)
{
    return rules->rules[LV-1].speed;
}

int GetBonus(const Rules *rules, int LV)
{
    return rules->rules[LV-1].bonus;
}

