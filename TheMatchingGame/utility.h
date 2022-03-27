#pragma once


void gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(hConsole, c);
}

int getX()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.X;
	return -1;
}

int getY()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.Y;
	return -1;
}

void printSpaces(int n)
{
    for (int i = 0; i < n; ++i)
        std::cout << " ";
}
void swapVals(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapLLVals(Node2D* n1, Node2D* n2)
{
    int temp = n1->data; 
    n1->data = n2->data; 
    n2->data = temp; 
}

void colorText(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}