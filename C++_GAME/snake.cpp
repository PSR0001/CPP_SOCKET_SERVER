#include <iostream>
#include <conio.h>
#include <winbase.h>
#include<windows.h>
#include <time.h>
using namespace std;
// global variables
bool gameOver;
const int width = 25;
const int height = 25;
int x, y, fruitX, fruitY, score;

// snake tail
int tailX[100], tailY[100];
int ntail;

enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;

// Functions
void setup()
{
    gameOver = false;
    dir = STOP;
    x = height / 2;
    y = width / 2;
    fruitX = rand() % height;
    fruitY = rand() % width;
    score = 0;
}
void draw()
{
    system("cls");
    // Uppder
    for (int i = 0; i < width + 1; i++)
    {
        cout << "#";
    }
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                cout << "#";

            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else
            {
                for (int k = 0; k < ntail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        cout << "o";
                    }
                }
                cout << " ";
            }
            if (j == (width - 1))
                cout << "#";
        }
        cout << endl;
    }

    // Lower
    for (int i = 0; i < width + 1; i++)
    {
        cout << "#";
    }
    cout << endl;

    cout << "Score :: " << score << endl;
}
void input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            dir = LEFT;
            break;

        case 'd':
            dir = RIGHT;
            break;

        case 's':
            dir = UP;
            break;

        case 'w':
            dir = DOWN;
            break;

        case 'x':
            gameOver = true;
            break;

        default:
            break;
        }
    }
    if (x > width || x < 0 || y > width || y < 0)
    {
        gameOver = true;
    }

    for (int i = 0; i < ntail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;


    if (x == fruitX && y == fruitY)
    {
        score++;
        fruitX = rand() % height;
        fruitY = rand() % width;
        ntail++;
    }
}
void logic()
{
    // tail logic
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 0; i < ntail; i++)
    {

        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;

    case RIGHT:
        x++;
        break;

    case UP:
        y++;
        break;

    case DOWN:
        y--;
        break;

    default:
        break;
    }
}

int main()
{

    setup();

    while (!gameOver)
    {
        draw();
        input();
        logic();
        Sleep(10);
    }

    return 0;
}
