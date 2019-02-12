#include "Block.h"
#include "Color.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

bool Add(Blocks *blocks, const Block *block)
{
    Block *p = (Block*)malloc(sizeof(Block) * (blocks->size+1));
    if(p==nullptr) return false;

    for(int i=0; i<blocks->size; i++)
    {
        p[i] = blocks->blocks[i];
    }
    p[blocks->size] = *block;
    free(blocks->blocks);
    blocks->blocks = p;
    blocks->size += 1;
    return true;
}

bool Read(FILE *fp, Blocks *blocks)
{
    if(!fp) return false;

    char str[10] = {};
    while(fgets(str, 10, fp)!=NULL)
    {
        Block block;
        sscanf(str, "%c", &block.type);

        // read shape
        for(int i=0; i<block.size; i++)
        {
            int tmp[4] = {0};
            fgets(str, 10, fp);
            sscanf(str, "%d %d %d %d", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);

            for(int j=0; j<block.size; j++)
            {
                block.shape[i][j] = tmp[j];
            }
        }
        if(!Add(blocks, &block)) break;
    }
    return true;
}

void Free(Blocks *blocks)
{
    if(blocks->size) free(blocks->blocks);
    blocks->blocks = 0;
    blocks->size = 0;
}

void Print(const Blocks *blocks)
{
    printf("Blocks' count: %d\n", blocks->size);
    for(int i=0; i<blocks->size; i++)
    {
        printf("Block's type: %c\n", blocks->blocks[i].type);
        for(int x=0; x<blocks->blocks[i].size; x++)
        {
            for(int y=0; y<blocks->blocks[i].size; y++)
            {
                printf("%2d", blocks->blocks[i].shape[x][y] ? 1 : 0);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void ResetPosition(ActiveBlock *block, int X)
{
    block->currX = X;
    block->currY = 0;
}

void InitBlock(const Blocks *blocks, ActiveBlock *block, int colorSize, BlockType type, int X)
{
    block->currX = X;
    block->currY = (type == NEW ? 0 : 1);
    block->colorID = rand() % (colorSize-SYSCOLORCOUNT)+SYSCOLORCOUNT;
    block->dir = CW;
    block->type = type;

    int shapeid = rand() % blocks->size;
    CopyBlock(&blocks->blocks[shapeid], &block->block);
}

void CopyBlock(const Block *ori, Block *newBlock)
{
    newBlock->size = ori->size;
    newBlock->type = ori->type;
    memcpy(newBlock->shape, ori->shape, BlockSize * BlockSize);
}

void moveDown(ActiveBlock *block)
{
    block->currY += 1;
}

void moveRight(ActiveBlock *block)
{
    block->currX += 1;
}

void moveLeft(ActiveBlock *block)
{
    block->currX -= 1;
}

void Rotate(ActiveBlock *block, int type, RotateDir dir)
{
    bool tmp[block->block.size][block->block.size];
    for(int i=0; i<block->block.size; i++)
    {
        for(int j=0; j<block->block.size; j++)
        {
            tmp[i][j] = false;
        }
    }

    for(int i = block->block.size-type; i < block->block.size; i++)
    {
        for(int j = block->block.size-type; j < block->block.size; j++)
        {
            switch (dir) {
                case CW:
                {
                    int k = block->block.size-i-(type%2==0?1:0);
                    tmp[j][k] = block->block.shape[i][j];
                    break;
                }
                case CCW:
                {
                    int k = block->block.size-j-(type%2==0?1:0);
                    tmp[k][i] = block->block.shape[i][j];
                    break;
                }
            }
        }
    }
    memcpy(block->block.shape, tmp, block->block.size*block->block.size);
}

// enum Shape {O, I, S, Z, L, J, T};
void Rotate(ActiveBlock *block)
{
    switch(block->block.type)
    {
        case 'I':
            Rotate(block, 4, block->dir);
            if(block->dir==CW) block->dir = CCW;
            else block->dir = CW;
            break;

        case 'S': case 'Z':
            Rotate(block, 4, block->dir);
            if(block->dir==CW) block->dir = CCW;
            else block->dir = CW;
            break;

        case 'L': case 'J': case 'T':
            Rotate(block, 3, block->dir);
            break;
    }
}

void CopyBlock(const ActiveBlock *ori, ActiveBlock *newBlock)
{
//    newBlock->currX = ori->currX;
//    newBlock->currY = ori->currY;
    newBlock->colorID = ori->colorID;
    newBlock->dir = ori->dir;
    CopyBlock(&ori->block, &newBlock->block);
}


void SwapBlock(ActiveBlock *cur, ActiveBlock *next)
{
    ActiveBlock tmp;
    CopyBlock(cur, &tmp);
    CopyBlock(next, cur);
    CopyBlock(&tmp, next);
}

//bool isLessOneLine(){ // check the second line
//    for(int j = 0; j < blockSizeX; j++){
//        if(map[2][newBlockInitX+j] > 0) return true;
//    }
//    return false;
//}
