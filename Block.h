#include <stdio.h>

#ifndef Block__
#define Block__

enum RotateDir {CW, CCW};
enum BlockType {NEW, NEXT};

constexpr int BlockSize = 4;

struct Block
{
    int size = BlockSize;
    char type = '\0';
    bool shape[BlockSize][BlockSize];
};

struct Blocks
{
    Block *blocks = 0;
    int size = 0;
};

struct ActiveBlock
{
    Block block;
    BlockType type = NEW;
    int currX = 0;
    int currY = 0;
    int colorID = 0;
    RotateDir dir = CW;
};

bool Add(Blocks *blocks, const Block *block);
bool Read(FILE *fp, Blocks *blocks);
void Free(Blocks *blocks);
void Print(const Blocks *blocks);
void CopyBlock(const Block *ori, Block *newBlock);
void CopyBlock(const ActiveBlock *ori, ActiveBlock *newBlock);

void SwapBlock(ActiveBlock *cur, ActiveBlock *next);
void InitBlock(const Blocks *blocks, ActiveBlock *block, int colorSize, BlockType type, int X);
void ResetPosition(ActiveBlock *block, int X);

void moveDown(ActiveBlock *block);
void moveRight(ActiveBlock *block);
void moveLeft(ActiveBlock *block);
void Rotate(ActiveBlock *block);

#endif /* Block__ */
