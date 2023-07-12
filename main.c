#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <math.h>


#define WIDTH 65
#define HEIGHT 25


typedef struct {
    int x, y;
    int w;
} TRacket;

typedef struct {
    float x, y;
    int ix, iy;
    float alfa, speed;

} TBall;


void moveRacket(int x);
void init(int level);
void show();
void initRacket();
void putRacket();
void setcur(int x, int y);
void initBall();
void putBall();
void moveBall(float x, float y);
void autoMoveBall();
void printNewLevel();
void printLooze();

char field [HEIGHT][WIDTH+1];   // +1 for symbol '\0'
TRacket racket;                // Global Racket
TBall ball;
int hitCnt = 0, maxHitCnt = 0, money = 0;
int level = 1;

int main(void)
{
    BOOL run = FALSE;

    initBall();
    initRacket();

    do
    {
        begin1:
        setcur(0, 0);

        if (run)
            autoMoveBall();

        if (ball.y > HEIGHT)
        {
            run = FALSE;
            if (hitCnt > maxHitCnt)
                maxHitCnt = hitCnt;
            hitCnt = 0;
            level = 1;
            printLoose();
        }

        if (hitCnt == level * 5 && level < 3)
        {
            level++;
            printNewLevel();
            run = FALSE;
            goto begin1;
        }

        init(level);
        putRacket();
        putBall();
        show();

        if (GetKeyState('A') < 0) moveRacket(racket.x - 1);
        if (GetKeyState('D') < 0) moveRacket(racket.x + 1);
        if (GetKeyState('W') < 0) run = TRUE;

        if (!run)
            moveBall(racket.x + racket.w/2, racket.y - 1);

        Sleep(1);
    }
    while (GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}

void init(int level)
{
    if (level == 1)
    {
        for (int i = 0; i < WIDTH; i++)
            field[0][i] = '#';
        field[0][WIDTH] = '\0';

        strncpy(field[1], field[0], WIDTH);

        for (int i = 1; i < WIDTH-1; i++)   //-1 because the last non-\0 symbol is #
        {
            field[1][i] = ' ';
        }

        for (int i = 2; i < HEIGHT; i++)
            strncpy(field[i], field[1], WIDTH);
    }
    else if (level == 2)
    {
        init(1);
        for (int i = 15; i < 50; i++)
            field[12][i] = '#';
    }
    else
    {
        init(2);

        for (int i = 3; i < 10; i++)
        {
            field[5][i] = '#';
            field[5][WIDTH-i-1] = '#';
        }
    }
}

void show()
{
    for (int i = 0; i < HEIGHT-1; i++)
    {
        printf("%s", field[i]);

        if (i == 3)
            printf("\t\tCURRENT HITS: %d", hitCnt);
        if (i == 4)
            printf("\t\tMAX HITS: %d", maxHitCnt);
        if (i == 5)
        {
            printf("\t\tCURRENT LEVEL: %d", level);

            if (level == 3)
                printf("(FINAL ENDLESS MODE!)");
        }
        printf("\n");
    }
    printf("%s", field[HEIGHT-1]);      //ÑÒÐÎÊÀ ÍÀ ÊÎÒÎÐÎÉ ÍÀÕÎÄÈÒÑß ÐÀÊÅÒÊÀ

}

void initRacket()
{
    racket.w = 7;
    racket.x = (WIDTH - racket.w) / 2;
    racket.y = HEIGHT - 2;
}

void putRacket()
{
    for (int i = racket.x; i < racket.x + racket.w; i++)
        field[racket.y][i] = '@';
}

void moveRacket(int x)
{
    racket.x = x;
    if (racket.x < 1)
        racket.x = 1;

    if (racket.x + racket.w >= WIDTH)
        racket.x = WIDTH - 1 - racket.w;
}

void setcur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void initBall()
{
    moveBall(2, 2);
    ball.alfa = M_PI/4;
    ball.speed = 0.44;
}

void putBall()
{
    field[ball.iy][ball.ix] = '*';
}

void moveBall(float x, float y)
{
    ball.x = x;
    ball.y = y;
    ball.ix = (int)round(ball.x);
    ball.iy = (int)round(ball.y);
}

void autoMoveBall()
{
    if (ball.alfa < 0) ball.alfa += M_PI * 2;
    if (ball.alfa > M_PI * 2) ball.alfa -= M_PI*2;

    TBall b1 = ball;
    moveBall(ball.x + cos(ball.alfa) * ball.speed,
             ball.y + sin(ball.alfa) * ball.speed);

    if (field[ball.iy][ball.ix] == '#' || field[ball.iy][ball.ix] == '@')
    {
        if (field[ball.iy][ball.ix] == '@')
                hitCnt++;

        if (ball.ix != b1.ix && ball.iy != b1.iy)
        {
            if (field[b1.iy][ball.ix] == field[ball.iy][b1.ix])
                b1.alfa = b1.alfa + M_PI;
            else
            {
                if (field[b1.iy][ball.ix] == '#')
                    b1.alfa = (2*M_PI - b1.alfa) + M_PI;
                else
                    b1.alfa = (2*M_PI - b1.alfa);
            }
        }
        else if (ball.iy == b1.iy)
            b1.alfa = (2*M_PI - b1.alfa) + M_PI;
        else
            b1.alfa = (2*M_PI - b1.alfa);

        ball = b1;
        autoMoveBall();
    }
}

void printNewLevel()
{
    for (int i = 0; i <= 26; i++)
        printf("\n");

    system("cls");
    printf("\n\n\n\n\n\n\n\n");
    printf("\t\t|    | ||||||| |       |   |     |||||| |     | |||||| |      ||\n");
    printf("\t\t||   | |       |       |   |     |      |     | |      |      ||\n");
    printf("\t\t| |  | |_ _ _   |  |  |    |     |_ _ _ |     | |_ _ _ |      ||\n");
    printf("\t\t|  | | |        |  |  |    |     |       |   |  |      |      ||\n");
    printf("\t\t|   || |         |   |     |     |        | |   |      |        \n");
    printf("\t\t|    | |_ _ _ 	 |   |     ||||| |_ _ _    |    |_ _ _ |||||  ()\n");
    Sleep(2250);
    system("cls");
}

void printLoose()
{
    system("cls");
    printf("\n\n\n\n\n\n\n\n");
    printf("____________________________________________________________________________|\n");
    printf("\t\t\t\tYOU LOOSE!!!                                | \n");
    printf("\t\t\t\tYOU LOOSE!!!                                | \n");
    printf("\t\t\t\tYOU LOOSE!!!                                |\n");
    printf("\t\t\t\tYOU LOOSE!!!                                |\n");
    printf("____________________________________________________________________________|");
    Sleep(2250);
    system("cls");
}
