#include "Board.h"
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

bool InitBoard(Board *board, int H, int W)
{
    Cell **p = (Cell **)malloc(sizeof(Cell *)*H);
    if(p==0) return false;

    for(int i=0; i<H; i++)
    {
        p[i] = (Cell *)malloc(sizeof(Cell)*W);
        if(p[i]==0)
        {
            board->h = i;
            if(i>0) return true;
            else return false;
        }
    }

    if(board->map!=nullptr) Free(board);
    board->map = p;
    board->h = H;
    board->w = W;
    board->windowsW = W*CellSize + (BlockSize+1)*CellSize;
    board->windowsH = H*CellSize;

    ClearBoard(board);
    return true;
}

void ClearBoard(Board *board)
{
    printf("%d %d\n", board->h, board->w);
    for(int i = 0; i < board->h; i++)
    {
        for(int j = 0; j < board->w; j++)
        {
            if(i==0 || j == 0 || i==board->h-1 || j==board->w-1)
            {
                board->map[i][j].isBlank = false;
                board->map[i][j].colorID = WHITE;
            }else{
                board->map[i][j].isBlank = true;
                board->map[i][j].colorID = GRAY;
            }
        }
    }
}

void Free(Board *board)
{
    for(int i=0; i<board->h; i++)
    {
        free(board->map[i]);
    }
    free(board->map);
    board->map = 0;
    board->h = 0;
    board->w = 0;
    board->windowsH = 0;
    board->windowsW = 0;
}

// Convert 0-600 to -1~1
Coordinate convertBlockToCoor(int winW, int winH, const Matrix *matrix)
{
    Coordinate co;

    // X: 0-600 —> -1 ~ +1
    co.x =  (float)(matrix->x - winW / 2)/ (winW / 2);

    // Y: 0-600 —> +1 ~ -1
    co.y = -(float)(matrix->y - winH / 2)/ (winH / 2);
    return co;
}

void DrawCell(const Matrix *matrix, int winH, int winW)
{
    // draw Rectangle: Convert array to -1~1 and draw
    int Grid[4][2] ={{0,0}, {0,1}, {1,1}, {1,0}};

    for(int k = 0; k < 4; k++)
    {
        Matrix mMatrix;
        mMatrix.x = (matrix->x+Grid[k][0])*CellSize;
        mMatrix.y = (matrix->y+Grid[k][1])*CellSize;

        Coordinate co = convertBlockToCoor(winW, winH, &mMatrix);
        glVertex2f(co.x, co.y);
    }
}

void DrawBoard(const Colors *colors, const Board *board)
{
    for(int i = 0; i < board->h; i++ )
    {
        for(int j = 0; j < board->w; j++)
        {
            if(board->map[i][j].isBlank)
            {
                glBegin(GL_LINE_LOOP);
            }
            else
            {
                glBegin(GL_QUADS);
            }
            Color color = GetColor(colors, board->map[i][j].colorID);
            glColor3f(color.r, color.g, color.b);

            Matrix matrix;
            matrix.x = j;
            matrix.y = i;

            DrawCell(&matrix, board->windowsH, board->windowsW);

            glEnd();
        }
    }
}

void DrawBlock(const Colors *colors, const Board *board, const ActiveBlock *block)
{
    for(int i = 0; i < block->block.size ; i++)
    {
        for(int j = 0; j < block->block.size; j++)
        {
            if(block->block.shape[i][j])
            {
                glBegin(GL_QUADS);

                Color color = GetColor(colors, block->colorID);
                glColor3f(color.r, color.g, color.b);

                Matrix matrix;
                matrix.x = block->currX+j;
                matrix.y = block->currY+i;
                DrawCell(&matrix, board->windowsH, board->windowsW);

                glEnd();
            }
        }
    }
}

bool hasCollision(const Board *board, const ActiveBlock *block, CollisionType checkCode)
{
    for(int i = 0; i < block->block.size; i++)
    {
        for(int j = 0; j < block->block.size; j++)
        {
            if(block->block.shape[i][j])
            {
                switch (checkCode) {
                    case LEFT:
                    {
                        if(!board->map[block->currY+i][block->currX+j-1].isBlank)
                        {
                            return true;
                        }
                        break;
                    }
                    case RIGHT:
                    {
                        if(!board->map[block->currY+i][block->currX+j+1].isBlank)
                        {
                            return true;
                        }
                        break;
                    }
                    case BOTTOM:
                    {
                        if(!board->map[block->currY+i+1][block->currX+j].isBlank)
                        {
                            return true;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            else
            {
                if(checkCode == ROTATE33 || checkCode == ROTATE44) {
                    if(checkCode == ROTATE33 && (i==0 || j==0)) continue;
                    if(!board->map[block->currY+i][block->currX+j].isBlank)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int Update(Board *board, const ActiveBlock *block)
{
    for(int i = 0; i < block->block.size; i++)
    {
        for(int j = 0; j < block->block.size; j++)
        {
            if(block->block.shape[i][j])
            {
                board->map[block->currY+i][block->currX+j].colorID = block->colorID;
                board->map[block->currY+i][block->currX+j].isBlank = false;
            }
        }
    }
    return ClearLine(board);
}

int ClearLine(Board *board)
{
    int ClearLineCount = 0;
    for(int i = board->h-2; i>0; ) // check bottom to top
    {
        bool isFull = true;
        for(int j = 1; j < board->w-1; j++) // check left to right
        {
            if(board->map[i][j].isBlank)
            {
                isFull = false;
                break;
            }
        }

        if(isFull) // need clear
        {
            ClearLineCount += 1;
            // move down
            for(int h=i-1; h>0; h--)
            {
                for(int w=1; w<board->w-1; w++)
                {
                    board->map[h+1][w] = board->map[h][w];
                    board->map[h][w].isBlank = true;
                    board->map[h][w].colorID = GRAY;
                }
            }
        }
        else
        {
            i--;
        }
    }
    return ClearLineCount;
}
