#include "Board.h"
#include "Block.h"
#include "Rule.h"

#ifndef Game__
#define Game__

struct Game
{
    int score = 0;
    int level = 0;
    int speed = 0;
    Rules rules;
};


void InitGame(Game *game, int score);
bool isGameOver(const Board *board);

bool pressDown(const Board *board, ActiveBlock *block);
void pressRight(const Board *board, ActiveBlock *block);
void pressLeft(const Board *board, ActiveBlock *block);
void pressSpace(const Board *board, ActiveBlock *block);
void pressRoate(const Board *board, ActiveBlock *block);

void SwapBlock(const Board *board, ActiveBlock *cur, ActiveBlock *next);

#endif /* Game__ */
