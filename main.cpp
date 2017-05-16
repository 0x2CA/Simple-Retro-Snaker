/*
* 作者：贺子一
* 日期：2017/3/36 22:36
*/


#include <graphics.h>      // 图形库
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

#define WIDTH_SQUARE 20 //正方形宽度
#define INTERVAL 4 //间隔
#define WIDTH_NUMBER 20 //宽数量
#define HEIGHT_NUMBER 20 //高数量
#define WIDTH_SQUARE_HALF (WIDTH_SQUARE>>1) //正方形宽度一半
#define WIDTH_INTERFACE ((WIDTH_SQUARE+ INTERVAL)*(WIDTH_NUMBER+2)+INTERVAL) //界面宽度
#define HEIGHT_INTERFACE ((WIDTH_SQUARE + INTERVAL) * (HEIGHT_NUMBER+2) + INTERVAL) //界面高度
#define BLANK 0 //空白
#define SNAKE 1 //蛇
#define FOOD 3 //食物
#define KEY_DIRECTION 0xe0 //方向键
#define KEY_UP 0x0048 //上
#define KEY_DOWN 0x0050 //下
#define KEY_LEFT 0x004b //左
#define KEY_RIGHT 0x004d //右
#define SPEED 500 //速度 微妙
#define  TRANSFORM(x) ((x-INTERVAL-WIDTH_SQUARE_HALF)/(INTERVAL+WIDTH_SQUARE)-1) //转换

void init();
void initdata();
void initsnake();
void Circulation_Subject();
void rand_food();
int input();
void walk(int widht = WIDTH_SQUARE);
void judge(int n);
void Draw_Wall();
void Draw_Square(int x, int y, int widht = WIDTH_SQUARE);
void Draw_Head(int x, int y, int direction, int widht = WIDTH_SQUARE);
void Draw_Body(int x, int y, int widht = WIDTH_SQUARE);
void Draw_Tail(int x, int y, int direction, int widht = WIDTH_SQUARE);
void Draw_Food(int x, int y, int widht = WIDTH_SQUARE);

typedef struct Data
{
	short type;
	int x;
	int y;
	Data *next;
	Data *prev;
}Data;

Data data[HEIGHT_NUMBER][WIDTH_NUMBER], *head, *tail;
int head_direction;//头方向 尾方向 

int main()
{
	initgraph(WIDTH_INTERFACE, HEIGHT_INTERFACE);   // 初始化界面大小
	if (MessageBox(NULL, TEXT("你准备好了吗？"), TEXT("结果"), MB_YESNO) == IDYES) {
		init();
	}
	else {
		closegraph();
		exit(0);
	}
	Circulation_Subject();//游戏循环
	closegraph();
	return 0;
}

//初始化
void init()
{
	cleardevice();
	BeginBatchDraw();
	Draw_Wall();	//绘制墙壁
	initdata();		//数据初始化
	initsnake();
	rand_food();
	FlushBatchDraw();
	EndBatchDraw();
}

//数据初始化
void initdata()
{
	for (int width = INTERVAL * 2 + WIDTH_SQUARE_HALF + WIDTH_SQUARE, k = 0; k < WIDTH_NUMBER; k++, width += INTERVAL + WIDTH_SQUARE)
	{
		for (int height = INTERVAL * 2 + WIDTH_SQUARE_HALF + WIDTH_SQUARE, j = 0; j < HEIGHT_NUMBER; j++, height += INTERVAL + WIDTH_SQUARE)
		{
			data[j][k].x = width;
			data[j][k].y = height;
			data[j][k].next = NULL;
			data[j][k].prev = NULL;
			data[j][k].type = BLANK;
		}
	}
	head_direction = KEY_RIGHT;
}

//初始蛇
void initsnake()
{
	int j = (HEIGHT_NUMBER - 1) >> 1, k = (WIDTH_NUMBER - 1) >> 1;
	data[j][k].type = SNAKE;
	tail = data[j][k].next = &data[j][k - 1];
	head = data[j][k].prev = &data[j][k + 1];
	head->next = tail->prev = &data[j][k];
	Draw_Head(head->x, head->y, head_direction);
	Draw_Body(head->next->x, head->next->y);
	head->type = SNAKE;
	tail->type = SNAKE;
	Draw_Tail(tail->x, tail->y, KEY_LEFT);
}

//随机食物
void rand_food()
{
	Data *tmp[WIDTH_NUMBER*HEIGHT_NUMBER];
	int n = 0;
	for (int width = INTERVAL * 2 + WIDTH_SQUARE_HALF + WIDTH_SQUARE, k = 0; k < WIDTH_NUMBER; k++, width += INTERVAL + WIDTH_SQUARE)
	{
		for (int height = INTERVAL * 2 + WIDTH_SQUARE_HALF + WIDTH_SQUARE, j = 0; j < HEIGHT_NUMBER; j++, height += INTERVAL + WIDTH_SQUARE)
		{
			if (data[j][k].type == BLANK)
			{
				tmp[n] = &data[j][k];
				n++;
			}
		}
	}
	srand((int)time(0));
	n = rand() % (n - 1) - 1;
	tmp[n]->type = FOOD;
	Draw_Food(tmp[n]->x, tmp[n]->y);
}

//循环主体
void Circulation_Subject()
{
	int time;
	BeginBatchDraw();
	while (TRUE)
	{
		time = input();
		walk();
		FlushBatchDraw();
		Sleep(time / 2);

	}
	EndBatchDraw();
}

//获取操作
int input()
{
	int key;
	for (int i = 0; i < 10; i++)
	{

		if (_kbhit())
		{
			key = _getch();
			if (key == KEY_DIRECTION)
			{
				key = _getch();
				switch (key)
				{
				case  KEY_UP:
					if (head_direction == key)
					{
						break;
					}
					head_direction = KEY_UP;
					return (SPEED - ((SPEED*i) / 10));
					break;
				case  KEY_DOWN:
					if (head_direction == key)
					{
						break;
					}
					head_direction = KEY_DOWN;
					return (SPEED - ((SPEED*i) / 10));
					break;
				case  KEY_LEFT:
					if (head_direction == key)
					{
						break;
					}
					head_direction = KEY_LEFT;
					return (SPEED - ((SPEED*i) / 10));
					break;
				case  KEY_RIGHT:
					if (head_direction == key)
					{
						break;
					}
					head_direction = KEY_RIGHT;
					return (SPEED - ((SPEED*i) / 10));
					break;
				default:
					break;
				}
			}
		}
		Sleep(SPEED / 10);
	}
	return 0;

}

//行走
void walk(int widht)
{
	switch (head_direction)
	{
	case  KEY_UP:
		judge(TRANSFORM(head->y) - 1);
		if (head->next == &data[TRANSFORM(head->y) - 1][TRANSFORM(head->x)])
		{
			head_direction = KEY_DOWN;
			walk();
			return;
		}
		if (data[TRANSFORM(head->y) - 1][TRANSFORM(head->x)].type == SNAKE)
		{
			if (MessageBox(NULL, TEXT("你输了!\n是否重新开始？"), TEXT("结果"), MB_YESNO) == IDYES) {
				init();
			}
			else {
				closegraph();
				exit(0);
			}
		}
		clearrectangle(head->x - (widht >> 1), head->y - (widht >> 1), head->x + (widht >> 1), head->y + (widht >> 1));
		head->prev = &data[TRANSFORM(head->y) - 1][TRANSFORM(head->x)];
		data[TRANSFORM(head->y) - 1][TRANSFORM(head->x)].next = head;
		Draw_Body(head->x, head->y);
		head = head->prev;
		if (head->type == FOOD)
		{
			clearrectangle(head->x - (widht >> 1), head->y - (widht >> 1), head->x + (widht >> 1), head->y + (widht >> 1));
			Draw_Head(head->x, head->y, KEY_UP);
			head->type = SNAKE;
			rand_food();
			return;
		}
		Draw_Head(head->x, head->y, KEY_UP);
		head->type = SNAKE;
		clearrectangle(tail->x - (widht >> 1), tail->y - (widht >> 1), tail->x + (widht >> 1), tail->y + (widht >> 1));
		tail = tail->prev;
		tail->next->prev = NULL;
		tail->next->type = BLANK;
		tail->next = NULL;
		clearrectangle(tail->x - (widht >> 1), tail->y - (widht >> 1), tail->x + (widht >> 1), tail->y + (widht >> 1));
		if (TRANSFORM(tail->x) - TRANSFORM(tail->prev->x) == -1)
		{
			Draw_Tail(tail->x, tail->y, KEY_LEFT);
		}
		else if (TRANSFORM(tail->x) - TRANSFORM(tail->prev->x) == 1)
		{
			Draw_Tail(tail->x, tail->y, KEY_RIGHT);

		}
		else if (TRANSFORM(tail->y) - TRANSFORM(tail->prev->y) == -1)
		{
			Draw_Tail(tail->x, tail->y, KEY_UP);
		}
		else
		{
			Draw_Tail(tail->x, tail->y, KEY_DOWN);

		}
		break;
	case  KEY_DOWN:
		judge(TRANSFORM(head->y) - HEIGHT_NUMBER);
		if (head->next == &data[TRANSFORM(head->y) + 1][TRANSFORM(head->x)])
		{
			head_direction = KEY_UP;
			walk();
			return;
		}
		if (data[TRANSFORM(head->y) + 1][TRANSFORM(head->x)].type == SNAKE)
		{
			if (MessageBox(NULL, TEXT("你输了!\n是否重新开始？"), TEXT("结果"), MB_YESNO) == IDYES) {
				init();
			}
			else {
				closegraph();
				exit(0);
			}
		}
		clearrectangle(head->x - (widht >> 1), head->y - (widht >> 1), head->x + (widht >> 1), head->y + (widht >> 1));
		head->prev = &data[TRANSFORM(head->y) + 1][TRANSFORM(head->x)];
		data[TRANSFORM(head->y) + 1][TRANSFORM(head->x)].next = head;
		Draw_Body(head->x, head->y);
		head = head->prev;
		if (head->type == FOOD)
		{
			clearrectangle(head->x - (widht >> 1), head->y - (widht >> 1), head->x + (widht >> 1), head->y + (widht >> 1));
			Draw_Head(head->x, head->y, KEY_DOWN);
			head->type = SNAKE;
			rand_food();
			return;
		}
		Draw_Head(head->x, head->y, KEY_DOWN);
		head->type = SNAKE;
		clearrectangle(tail->x - (widht >> 1), tail->y - (widht >> 1), tail->x + (widht >> 1), tail->y + (widht >> 1));
		tail = tail->prev;
		tail->next->prev = NULL;
		tail->next->type = BLANK;
		tail->next = NULL;
		clearrectangle(tail->x - (widht >> 1), tail->y - (widht >> 1), tail->x + (widht >> 1), tail->y + (widht >> 1));
		if (TRANSFORM(tail->x) - TRANSFORM(tail->prev->x) == -1)
		{
			Draw_Tail(tail->x, tail->y, KEY_LEFT);
		}
		else if (TRANSFORM(tail->x) - TRANSFORM(tail->prev->x) == 1)
		{
			Draw_Tail(tail->x, tail->y, KEY_RIGHT);

		}
		else if (TRANSFORM(tail->y) - TRANSFORM(tail->prev->y) == -1)
		{
			Draw_Tail(tail->x, tail->y, KEY_UP);
		}
		else
		{
			Draw_Tail(tail->x, tail->y, KEY_DOWN);

		}
		break;
	case  KEY_LEFT:
		judge(TRANSFORM(head->x) - 1);
		if (head->next == &data[TRANSFORM(head->y)][TRANSFORM(head->x) - 1])
		{
			head_direction = KEY_RIGHT;
			walk();
			return;
		}
		if (data[TRANSFORM(head->y)][TRANSFORM(head->x) - 1].type == SNAKE)
		{
			if (MessageBox(NULL, TEXT("你输了!\n是否重新开始？"), TEXT("结果"), MB_YESNO) == IDYES) {
				init();
			}
			else {
				closegraph();
				exit(0);
			}
		}
		clearrectangle(head->x - (widht >> 1), head->y - (widht >> 1), head->x + (widht >> 1), head->y + (widht >> 1));
		head->prev = &data[TRANSFORM(head->y)][TRANSFORM(head->x) - 1];
		data[TRANSFORM(head->y)][TRANSFORM(head->x) - 1].next = head;
		Draw_Body(head->x, head->y);
		head = head->prev;
		if (head->type == FOOD)
		{
			clearrectangle(head->x - (widht >> 1), head->y - (widht >> 1), head->x + (widht >> 1), head->y + (widht >> 1));
			Draw_Head(head->x, head->y, KEY_LEFT);
			head->type = SNAKE;
			rand_food();
			return;
		}
		Draw_Head(head->x, head->y, KEY_LEFT);
		head->type = SNAKE;
		clearrectangle(tail->x - (widht >> 1), tail->y - (widht >> 1), tail->x + (widht >> 1), tail->y + (widht >> 1));
		tail = tail->prev;
		tail->next->prev = NULL;
		tail->next->type = BLANK;
		tail->next = NULL;
		clearrectangle(tail->x - (widht >> 1), tail->y - (widht >> 1), tail->x + (widht >> 1), tail->y + (widht >> 1));
		if (TRANSFORM(tail->x) - TRANSFORM(tail->prev->x) == -1)
		{
			Draw_Tail(tail->x, tail->y, KEY_LEFT);
		}
		else if (TRANSFORM(tail->x) - TRANSFORM(tail->prev->x) == 1)
		{
			Draw_Tail(tail->x, tail->y, KEY_RIGHT);

		}
		else if (TRANSFORM(tail->y) - TRANSFORM(tail->prev->y) == -1)
		{
			Draw_Tail(tail->x, tail->y, KEY_UP);
		}
		else
		{
			Draw_Tail(tail->x, tail->y, KEY_DOWN);

		}
		break;
	case  KEY_RIGHT:
		judge(TRANSFORM(head->x) - WIDTH_NUMBER);
		if (head->next == &data[TRANSFORM(head->y)][TRANSFORM(head->x) + 1])
		{
			head_direction = KEY_LEFT;
			walk();
			return;
		}
		if (data[TRANSFORM(head->y)][TRANSFORM(head->x) + 1].type == SNAKE)
		{
			if (MessageBox(NULL, TEXT("你输了!\n是否重新开始？"), TEXT("结果"), MB_YESNO) == IDYES) {
				init();
			}
			else {
				closegraph();
				exit(0);
			}
		}
		clearrectangle(head->x - (widht >> 1), head->y - (widht >> 1), head->x + (widht >> 1), head->y + (widht >> 1));
		head->prev = &data[TRANSFORM(head->y)][TRANSFORM(head->x) + 1];
		data[TRANSFORM(head->y)][TRANSFORM(head->x) + 1].next = head;
		Draw_Body(head->x, head->y);
		head = head->prev;
		if (head->type == FOOD)
		{
			clearrectangle(head->x - (widht >> 1), head->y - (widht >> 1), head->x + (widht >> 1), head->y + (widht >> 1));
			Draw_Head(head->x, head->y, KEY_RIGHT);
			head->type = SNAKE;
			rand_food();
			return;
		}
		Draw_Head(head->x, head->y, KEY_RIGHT);
		head->type = SNAKE;
		clearrectangle(tail->x - (widht >> 1), tail->y - (widht >> 1), tail->x + (widht >> 1), tail->y + (widht >> 1));
		tail = tail->prev;
		tail->next->prev = NULL;
		tail->next->type = BLANK;
		tail->next = NULL;
		clearrectangle(tail->x - (widht >> 1), tail->y - (widht >> 1), tail->x + (widht >> 1), tail->y + (widht >> 1));
		if (TRANSFORM(tail->x) - TRANSFORM(tail->prev->x) == -1)
		{
			Draw_Tail(tail->x, tail->y, KEY_LEFT);
		}
		else if (TRANSFORM(tail->x) - TRANSFORM(tail->prev->x) == 1)
		{
			Draw_Tail(tail->x, tail->y, KEY_RIGHT);

		}
		else if (TRANSFORM(tail->y) - TRANSFORM(tail->prev->y) == -1)
		{
			Draw_Tail(tail->x, tail->y, KEY_UP);
		}
		else
		{
			Draw_Tail(tail->x, tail->y, KEY_DOWN);

		}
		break;
	default:
		break;
	}

}

//判断结果
void judge(int n)
{
	int j, k;
	if (n == -1)
	{
		if (MessageBox(NULL, TEXT("你输了!\n是否重新开始？"), TEXT("结果"), MB_YESNO) == IDYES) {
			init();
		}
		else {
			closegraph();
			exit(0);
		}
		return;
	}
	for (j = 0; j < HEIGHT_NUMBER; j++)
	{
		for (k = 0; k< WIDTH_NUMBER; k++)
		{
			if (data[j][k].type == BLANK || data[j][k].type == FOOD)
			{
				return;
			}
		}
	}
	if (MessageBox(NULL, TEXT("你赢了!\n是否重新开始？"), TEXT("结果"), MB_YESNO) == IDYES) {
		init();
	}
	else {
		closegraph();
		exit(0);
	}

}

//绘制墙壁
void Draw_Wall()
{
	setcolor(RED);
	for (int width = INTERVAL + WIDTH_SQUARE_HALF; width < WIDTH_INTERFACE; width += INTERVAL + WIDTH_SQUARE)
	{
		Draw_Square(width, INTERVAL + WIDTH_SQUARE_HALF);
		Draw_Square(width, HEIGHT_INTERFACE - INTERVAL - WIDTH_SQUARE_HALF);
	}
	for (int height = INTERVAL * 2 + WIDTH_SQUARE_HALF + WIDTH_SQUARE; height < HEIGHT_INTERFACE - INTERVAL - WIDTH_SQUARE_HALF; height += INTERVAL + WIDTH_SQUARE)
	{
		Draw_Square(INTERVAL + WIDTH_SQUARE_HALF, height);
		Draw_Square(WIDTH_INTERFACE - INTERVAL - WIDTH_SQUARE_HALF, height);
	}
	setcolor(WHITE);
}

//绘制正方形 x y 为中心点
void Draw_Square(int x, int y, int widht)
{
	rectangle(x - (widht >> 1), y - (widht >> 1), x + (widht >> 1), y + (widht >> 1));
}

//绘制头
void Draw_Head(int x, int y, int direction, int widht)
{
	switch (direction)
	{
	case  KEY_UP:
		roundrect(x - (widht >> 1), y - (widht >> 2), x + (widht >> 1), y + (widht >> 1), INTERVAL * 2, INTERVAL);
		break;
	case  KEY_DOWN:
		roundrect(x - (widht >> 1), y - (widht >> 1), x + (widht >> 1), y + (widht >> 2), INTERVAL * 2, INTERVAL);
		break;
	case  KEY_LEFT:
		roundrect(x - (widht >> 2), y - (widht >> 1), x + (widht >> 1), y + (widht >> 1), INTERVAL * 2, INTERVAL);
		break;
	case  KEY_RIGHT:
		roundrect(x - (widht >> 1), y - (widht >> 1), x + (widht >> 2), y + (widht >> 1), INTERVAL * 2, INTERVAL);
		break;
	default:
		break;
	}
}

//绘制身体
void Draw_Body(int x, int y, int widht)
{
	roundrect(x - (widht >> 1), y - (widht >> 1), x + (widht >> 1), y + (widht >> 1), INTERVAL * 2, INTERVAL * 2);
}

//绘制尾
void Draw_Tail(int x, int y, int direction, int widht)
{
	int pts[6];
	switch (direction)
	{
	case  KEY_UP:
		pts[0] = x;
		pts[1] = y - (widht >> 1);
		pts[2] = x + (widht >> 1);
		pts[4] = x - (widht >> 1);
		pts[3] = pts[5] = y + (widht >> 1);
		break;
	case  KEY_DOWN:
		pts[0] = x;
		pts[1] = y + (widht >> 1);
		pts[2] = x - (widht >> 1);
		pts[4] = x + (widht >> 1);
		pts[3] = pts[5] = y - (widht >> 1);
		break;
	case  KEY_LEFT:
		pts[0] = x - (widht >> 1);
		pts[1] = y;
		pts[3] = y - (widht >> 1);
		pts[5] = y + (widht >> 1);
		pts[2] = pts[4] = x + (widht >> 1);
		break;
	case  KEY_RIGHT:
		pts[0] = x + (widht >> 1);
		pts[1] = y;
		pts[3] = y + (widht >> 1);
		pts[5] = y - (widht >> 1);
		pts[2] = pts[4] = x - (widht >> 1);
		break;
	default:
		return;
		break;
	}
	polygon((POINT*)pts, 3);
}

//绘制食物
void Draw_Food(int x, int y, int widht)
{
	circle(x, y, widht >> 1);
}
