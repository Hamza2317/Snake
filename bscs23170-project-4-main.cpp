#include<iostream>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>

using namespace std;

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}



struct position
{
	int ri, ci;
};

enum direction
{
	UP, DOWN, LEFT, RIGHT
};

struct snake
{
	position* ps;
	int size;
	int colour;

	direction DIR;

	int leftkey;
	int rightkey;
	int upkey;
	int downkey;

};

void init(snake& S, int rows, int cols)
{
	S.ps = new position[3]{ {rows / 2,cols / 2},{rows / 2,cols / 2 - 1},{rows / 2, cols / 2 - 2} };
	S.size = 3;
	S.DIR = RIGHT;
}

void displaysnake(snake& S, char sym = -37)
{
	for (int i = 0; i < S.size; i++)
	{
		gotoRowCol(S.ps[i].ri, S.ps[i].ci);
		cout << sym;
	}
}

void movesnake(snake& S, int rows, int cols)
{
	for (int i = S.size - 1; i - 1 >= 0; i--)
	{
		S.ps[i] = S.ps[i - 1];
	}

	switch (S.DIR)
	{
	case RIGHT:
		S.ps[0].ci++;
		if (S.ps[0].ci == cols)
			S.ps[0].ci = 0;
		break;
	case LEFT:
		S.ps[0].ci--;
		if (S.ps[0].ci == cols)
		{
			S.ps[0].ci = cols - 1;
		}
		break;
	case UP:
		S.ps[0].ri--;
		break;
	case DOWN:
		S.ps[0].ri++;
		break;
	}
}

void changedirection(snake& S, int key)
{
	switch (key)
	{
	case 72:
		if (S.DIR == DOWN)
			return;
		S.DIR = UP;
		break;
	case 80:
		S.DIR = DOWN;
		break;
	case 77:
		S.DIR = RIGHT;
		break;
	case 75:
		S.DIR = LEFT;
		break;
	}
}

void displayfood(position& food)
{
	gotoRowCol(food.ri, food.ci);
	cout << (char)3;
}


bool foodeaten(snake& S, position f)
{

	return S.ps[0].ri == f.ri && S.ps[0].ci == f.ci;
}

void growsnake(snake& S, int rows, int cols)
{

	position* ps = new position[S.size + 1];
	for (int i = 0; i < S.size; i++)
	{
		ps[i] = S.ps[i];
	}

	ps[S.size] = ps[S.size - 1];
	delete[] S.ps;
	S.ps = ps;
	S.size++;

}

bool validfood(const position& f, snake& S)
{
	for (int i = 0; i < S.size; i++)
	{
		if (S.ps[i].ri == f.ri && S.ps[i].ci == f.ci)
			return false;
	}
	return true;

}

void getfood(snake& S, position& food, int rows, int cols)
{
	do
	{

		food.ri = rand() % rows;
		food.ci = rand() % cols;
	} while (!validfood(food, S));

}

int main()
{
	int rows = 30;
	int cols = 30;
	position food;
	snake S;
	init(S, rows, cols);

	getfood(S, food, rows, cols);
	displayfood(food);

	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();
			if (key == 224)
			{
				key = _getch();
				changedirection(S, key);
			}
		}


		displaysnake(S);
		Sleep(100);
		displaysnake(S, ' ');
		movesnake(S, rows, cols);

		if (foodeaten(S, food))
		{
			growsnake(S, rows, cols);
			getfood(S, food, rows, cols);
			displayfood(food);
		}



	}
}