#include "Game.h"

void InitGame(Game *game, int score)
{
    game->score = score;
    game->level = GetLevel(&game->rules, score);
    game->speed = GetSpeed(&game->rules, game->level);
}

bool pressDown(const Board *board, ActiveBlock *block)
{
    if(!hasCollision(board, block, BOTTOM))
    {
        moveDown(block);
        return true;
    }
    return false;
}

void pressRight(const Board *board, ActiveBlock *block)
{
    if(!hasCollision(board, block, RIGHT))
    {
        moveRight(block);
    }
}

void pressLeft(const Board *board, ActiveBlock *block)
{
    if(!hasCollision(board, block, LEFT))
    {
        moveLeft(block);
    }
}

void pressSpace(const Board *board, ActiveBlock *block)
{
    if(!isGameOver(board))
    {
        while(pressDown(board, block));
    }
}

void pressRoate(const Board *board, ActiveBlock *block)
{
    // type: O, I, S, Z, L, J, T
    switch(block->block.type)
    {
        case 'I':
            if(!hasCollision(board, block, ROTATE44))
            {
                Rotate(block);
            }
            break;
        case 'S': case 'Z':
        case 'L': case 'J': case 'T':
            if(!hasCollision(board, block, ROTATE33))
            {
                Rotate(block);
            }
            break;
        default:
            break;
    }
}

bool isGameOver(const Board *board)
{
    // check the top line
    for(int j = 1; j < board->w-1; j++)
    {
        if(!board->map[1][j].isBlank) return true;
    }
    return false;
}

void SwapBlock(const Board *board, ActiveBlock *cur, ActiveBlock *next)
{
    ActiveBlock tmp;
    CopyBlock(next, &tmp);
    tmp.currY = cur->currY;
    tmp.currX = cur->currX;
    
    bool testA = hasCollision(board, &tmp, LEFT);
    bool testC = hasCollision(board, &tmp, BOTTOM);
    
    if(!testA && !testC)
    {
        SwapBlock(cur, next);
    }
}
