#ifndef GameBoard__
#define GameBoard__
#include "Color.h"
#include "Block.h"

enum CollisionType {RIGHT, LEFT, BOTTOM, ROTATE33, ROTATE44, COLLISIONCOUNT};
constexpr int CellSize = 32;

struct Cell
{
    int colorID = WHITE;
    bool isBlank = true;
};

struct Board
{
    Cell **map = nullptr;
    int w = 0;
    int h = 0;
    int windowsW = 0;
    int windowsH = 0;
};

struct Coordinate
{
    float x = 0.0;
    float y = 0.0;
};

struct Matrix
{
    int x = 0;
    int y = 0;
};

void Draw(const Board *board);
void ClearBoard(Board *board);
void Free(Board *board);
bool InitBoard(Board *board, int H, int W);
void DrawCell(const Matrix *matrix, int winH, int winW);
void DrawBoard(const Colors *colors, const Board *board);
void DrawBlock(const Colors *colors, const Board *board, const ActiveBlock *block);
bool hasCollision(const Board *board, const ActiveBlock *block, CollisionType checkCode);

int ClearLine(Board *board);
int Update(Board *board, const ActiveBlock *block);

#endif /* GameBoard__ */
