#include <stdlib.h>
#include <ege.h>
#include "Button.h"
#include<graphics.h>
#define MAP_W 50
#define MAP_H 40
int startflag = 0;
int hardflag = 1;
const color_t GCOLOR[] = {BLACK, GREEN, RED};
color_t uicolor = EGERGB(0xAD, 0xD8, 0xE6);
int gw, gh;

void reselectbutton(){
    setfillcolor(uicolor);
    ege_fillrect(19*50,25,300,20);
    xyprintf(19*50,25,"当前难度：%s",hardflag?"hard":"easy");
    setfillcolor(WHITE);  
    ege_fillrect(19*55, 200, 95,20);
    std::string a = "选择难度";
    setcolor(RED);  
    setbkmode(TRANSPARENT);
    setfont(20, 0, "宋体"); 
    int textWidth = textwidth(a.c_str());
    int textHeight = textheight("A"); 
    int textX = 19*55 + (95 - textWidth) / 2;
    int textY = 200 + (20 + textHeight) / 2 - textHeight; 
    outtextxy(textX, textY, a.c_str());
}
struct SNAKE
{

    int dir, head, inc, tail;
    int pool[MAP_W * MAP_H];
    PIMAGE snakeheaddown = newimage();
    PIMAGE snakeheadup = newimage();
    PIMAGE snakeheadright = newimage();
    PIMAGE snakeheadleft = newimage();
    PIMAGE snaketail = newimage();
    PIMAGE fruit = newimage();
    PIMAGE wall = newimage();
    int flagfru = getimage(fruit, "china.png");
    int flagup = getimage(snakeheadup, "snakeHeadUp.png");
    int flagdown = getimage(snakeheaddown, "snakeHeadDown.png");
    int flagright = getimage(snakeheadright, "snakeHeadRight.png");
    int flagleft = getimage(snakeheadleft, "snakeHeadLeft.png");
    int flagtail = getimage(snaketail, "snakeTail.png");
    int flagwall = getimage(wall, "wall.png");
    int score = -1;
} game;

void startpush()
{
    startflag = 1;
    return;
}
void backtoeasy()
{
    setfillcolor(uicolor);
    ege_fillrect(19 * 55, 220, 95, 40);
    hardflag = 0;
    reselectbutton();

    
}

void backtohard()
{
    setfillcolor(uicolor);
    ege_fillrect(19 * 55, 220, 95, 40);
    hardflag = 1;
    reselectbutton();
}

void drawselect()
{   
    setcolor(DARKGRAY);
    setfillcolor(EGEARGB(60,0xAD, 0xEF, 0xE6));
    ege_fillrect(19 * 55, 200, 95, 20);
    
    Button easy = Button(19 * 55, 200 + 20, 95, 20, backtoeasy,"easy");
    Button hard = Button(19 * 55, 240, 95, 20, backtohard,"hard");
    easy.text = "easy";
    hard.text = "hard";
    easy.draw(), hard.draw();
    mouse_msg msg;
    while (1)
    {
        msg = getmouse();
        if (easy.checkClick(msg.x, msg.y) && msg.is_left())
        {
            
            easy.onClick();
            break;
        }
        if (hard.checkClick(msg.x, msg.y) && msg.is_left())
        {
            hard.onClick();
            break;
        }
    }
}


inline void drawhead(const int &i)
{
    int x = (i % MAP_W) * gw, y = (i / MAP_W) * gh;
    switch (game.dir)
    {
    case 1:
        putimage(x, y, game.snakeheadup);
        break;
    case 4:
        putimage(x, y, game.snakeheadleft);
        break;
    case 6:
        putimage(x, y, game.snakeheadright);
        break;
    case 9:
        putimage(x, y, game.snakeheaddown);
        break;
    }
}

inline void drawAt(const int &i)
{
    int x = (i % MAP_W) * gw, y = (i / MAP_W) * gh;
    switch (game.pool[i] >> 16)
    {
    case 0:
        setfillcolor(GCOLOR[0]);
        bar(x, y, x + gw + 1, y + gh + 1);
        setfillcolor(WHITE);
        break;
    case 1:
        putimage(x, y, game.snaketail);
        break;
    case 2:
        putimage(x, y, game.fruit);
        break;
    case 3:
        drawhead(i);
        break;
    case 5:
        putimage(x, y, game.wall);
        break;
    }
}

void newFruit(void)
{
    int nf;
    int nw;
    game.score+=1;
    setfillcolor(uicolor);
    bar(19*50,0,350+19*50,20);
    setfont(20,0,"宋体");
    setbkmode(TRANSPARENT);
    setcolor(RED);
    xyprintf(19*50,2,"your score: %d",game.score);
    
    while (game.pool[nf = random(MAP_W * MAP_H)] >> 16);
    game.pool[nf] = 0x20000, drawAt(nf);
    if (hardflag)
    {
        while (game.pool[nw = random(MAP_W * MAP_H)] >> 16);
        game.pool[nw] = 0x50000, drawAt(nw);
    }
}

int moveSnake(const int dx, const int dy, const bool u = false)
{
    if (u && dx + (game.dir & 3) == 1 && dy + (game.dir >> 2) == 1)
        return 1;
    int nh;
    if (dx && !dy)
    {
        nh = game.head % MAP_W + dx;
        if (nh < 0 || nh >= MAP_W)
            return 0;
        nh = game.head + dx;
    }
    else
    {
        nh = game.head / MAP_W + dy;
        if (nh < 0 || nh >= MAP_H)
            return 0;
        nh = game.head + dy * MAP_W;
    }
    int s = game.pool[nh] >> 16;
    if (s == 1 || s == 3 || s == 5)
        return 0;
    if (s == 2)
        game.inc += 5, newFruit();
    if (game.inc > 0)
        --game.inc;
    else
    {
        game.tail = game.pool[s = game.tail] & 0xffff;
        game.pool[s] = 0, drawAt(s);
    }
    int pos = game.head;
    game.pool[game.head] |= nh;
    game.pool[game.head] -= 131072;
    game.pool[game.head = nh] = 0x30000;
    game.dir = (dx + 1) | ((dy + 1) << 2); // right:6 down:9 left:4 up:1
    drawAt(nh);
    drawAt(pos);
    return 1;
}

void gameInit(void)
{
    int data[] = {6, 0, 2, 0, 0x10000};
    memset(game.pool, 0, sizeof(game.pool));
    memmove(&game, data, sizeof(data));
}

void gameScene(void)
{   
    setcolor(RED);
    setfont(20,0,"宋体");
    setbkmode(TRANSPARENT);
    xyprintf(19*50,25,"当前难度:hard");
    Button start = Button(19 * 55, 600, 95, 30, startpush,"start");
    Button select = Button(19 * 55, 200, 95, 20, drawselect,"选择难度");
    setfillcolor(GCOLOR[0]);
    bar(0, 0, gw, gh);
    newFruit();
    select.draw();
    start.draw();
    mouse_msg msg;
    for (int c = -1; is_run(); delay_fps(60), --c)
    {
        while (!startflag)
        {
            msg = getmouse();
            if (start.checkClick(msg.x, msg.y) && msg.is_left())
            {
                start.onClick();
            }
            if (select.checkClick(msg.x, msg.y) && msg.is_left())
            {
                select.onClick();
            }
        }
        while (kbhit())
        {
            int key = getch() | 0x20;
            if (key == (27 | 0x20))
                return;
            if (key == 'a' || key == 'd')
            {
                if (!moveSnake(((key - 'a') >> 1 << 1) - 1, 0, true))
                    return;
            }
            else if (key == 's' || key == 'w')
            {
                if (!moveSnake(0, 1 - ((key - 's') >> 2 << 1), true))
                    return;
            }
        }
        if (c < 0)
        {
            if (!moveSnake((game.dir & 3) - 1, (game.dir >> 2) - 1))
                return;
            c = 20;
        }
    }
}

int main(void)
{
    setbkcolor(WHITE);
    setinitmode(INIT_ANIMATION);
    setcaption("贪吃蛇v1.0.0_powered_by_XRH23331153");
    initgraph(19 * 50 + 19 * 20, 20 * 40);
    setfillcolor(uicolor);
    bar(19 * 50, 0, 19 * 70, 20 * 40);
    gw = 19 * 50 / MAP_W, gh = getheight() / MAP_H;
    randomize();
    gameInit();
    gameScene();
    return 0;
}
