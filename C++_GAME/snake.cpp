#include <iostream>
#include <conio.h>
#include <winbase.h>
using namespace std;
// global variables
bool gameOver;
const int width = 25;
const int height = 25;
int x, y, fruitX, fruitY, score;
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
                cout << " ";

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
    if (x == fruitX && y == fruitY)
    {
        score++;
        fruitX = rand() % height;
        fruitY = rand() % width;
    }
}
void logic()
{
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
