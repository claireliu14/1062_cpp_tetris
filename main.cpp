#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "Color.h"
#include "Block.h"
#include "Board.h"
#include "Game.h"

Game mGame;
Blocks blocks;
Colors mColors;
Board mBoard;
ActiveBlock curBlock, nextBlock;

static int RESETTIME = 0;
int nextBlockX = 0;
int curBlockX = 0;

void PrintMessage()
{
    mGame.level = GetLevel(&mGame.rules, mGame.score);
    mGame.speed = GetSpeed(&mGame.rules, mGame.level);

    system("cls");
    printf("Welcome to my 2D Tetris! Please enjoy it\n\n"
           "Press down to move down the block\n"
           "Press left  <-- to move left\n"
           "Press right --> to move right\n"
           "Press top  to rotate the block\n"
           "Press [space] to drop down the block quickly\n"
           "Press [s] to swap the block\n"
           "Press [r] to reset the game\n"
           "Press [e] to exit\n\n");

    printf("Your Level: %d\tScore: %d\n", mGame.level, mGame.score);
}

static void display()
{
    // setting the background color to white
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // use the COLOR to clear the window background (it is an active)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw bottom blocks
    DrawBoard(&mColors, &mBoard);

    // appear new block
    DrawBlock(&mColors, &mBoard, &curBlock);
    DrawBlock(&mColors, &mBoard, &nextBlock);

    glutSwapBuffers();
}

void CheckUpdate()
{
    int line = Update(&mBoard, &curBlock);
    int total = UpdateScore(&mGame.rules, mGame.level, line);
    if(total > 0)
    {
        mGame.score += total;
        PrintMessage();
    }
    if(!isGameOver(&mBoard))
    {
        SwapBlock(&curBlock, &nextBlock);
        curBlock.currX = curBlockX;
        curBlock.currY = 0;
        InitBlock(&blocks, &nextBlock, mColors.size, NEXT, nextBlockX);
    }
}

void callback(int value)
{
    if(!isGameOver(&mBoard))
    {
        if(value == RESETTIME) // dynamic call by ms
        {
            glutTimerFunc(mGame.speed, callback, RESETTIME);
            if(!pressDown(&mBoard, &curBlock))
            {
                CheckUpdate();
            }
        }
        glutPostRedisplay();
    }
    else
    {
        system("cls");
        printf("------- G A M E  O V E R -------\n\n"
               "Your Level: %d\tScore: %d\n"
               "You can press [r] to restart or press [e] to exit.\n", mGame.level, mGame.score);
    }
}
void CloseGame()
{
    Free(&mColors);
    Free(&blocks);
    Free(&mBoard);
    Free(&mGame.rules);
}

static void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 32:
            pressSpace(&mBoard, &curBlock);
            CheckUpdate();
            break;

        case 'R': case 'r':
            RESETTIME+=1;
            InitGame(&mGame, 0);
            ClearBoard(&mBoard);
            InitBlock(&blocks, &curBlock, mColors.size, NEW, curBlockX);
            InitBlock(&blocks, &nextBlock, mColors.size, NEXT, nextBlockX);
            PrintMessage();
            glutTimerFunc(mGame.speed, callback, RESETTIME);
            break;

        case 'E': case 'e':
            CloseGame();
            exit(0);
            break;

        case 'S': case 's':
            SwapBlock(&mBoard, &curBlock, &nextBlock);
            break;
    }
    glutPostRedisplay();
}

void specialKey(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            pressLeft(&mBoard, &curBlock);
            break;

        case GLUT_KEY_UP:
            pressRoate(&mBoard, &curBlock);
            break;

        case GLUT_KEY_RIGHT:
            pressRight(&mBoard, &curBlock);
            break;

        case GLUT_KEY_DOWN:
            if(!pressDown(&mBoard, &curBlock))
            {
                CheckUpdate();
            }
            break;
    }
    glutPostRedisplay();
}

bool ReadFiles()
{
    FILE *fp = fopen("colors.txt", "r");
    if(!fp)
    {
        printf("colors.txt does not exist.\n");
        return false;
    }
    Read(fp, &mColors);
    fclose(fp);
    // Print(&mColors);

    fp = fopen("blocks.txt", "r");
    if(!fp)
    {
        printf("blocks.txt does not exist.\n");
        return false;
    }
    Read(fp, &blocks);
    fclose(fp);
    // Print(&blocks);

    fp = fopen("rules.txt", "r");
    if(!fp)
    {
        printf("rules.txt does not exist.\n");
        return false;
    }
    Read(fp, &mGame.rules);
    fclose(fp);
    // Print(&mGame.rules);

    return true;
}

int main(int argc, char *argv[])
{
   srand(time(0));

    if(ReadFiles())
    {
        // read grid size
        int h = 0, w = 0;
        printf("Welcome to the Tetris Game!\n"
               "Please intput the size of game board (Height x Width)\n"
               "The size can ONLY be set int range 10 ~ 25 > ");
        scanf("%d %d", &h, &w);
        while(!(h>=10 && h<=25 && w>=10 && w<=25))
        {
            printf("\nPlease input the VALID size (Height x Width) > ");
            scanf("%d %d", &h, &w);
        }

        // initialize environment
        InitGame(&mGame, 0);
        InitBoard(&mBoard, h+2, w+2);

        curBlockX = mBoard.w/2-curBlock.block.size/2;
        nextBlockX = mBoard.w;

        InitBlock(&blocks, &curBlock, mColors.size, NEW, curBlockX);
        InitBlock(&blocks, &nextBlock, mColors.size, NEXT, nextBlockX);
        PrintMessage();
    }
    else
    {
        printf("Oops! The Game cannot start... Please check files.\n");
        system("pause");
        return 0;
    }

    glutInit(&argc, argv);
    glutInitWindowSize(mBoard.windowsW, mBoard.windowsH);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("2D Tetris");

    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(mGame.speed, callback, RESETTIME);
    // glutIdleFunc(idle);

    glutMainLoop();

    CloseGame();

    return EXIT_SUCCESS;
}

