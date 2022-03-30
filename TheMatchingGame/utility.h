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

void colorText(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printSpaces(int n)
{
    for (int i = 0; i < n; ++i)
        std::cout << " ";
}

//modify values functions
void swapVals(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapPoints(Point& p1, Point& p2)
{
	Point temp = p1; 
	p1 = p2; 
	p2 = temp; 
}
void swapLLVals(Node2D* n1, Node2D* n2)
{
    int temp = n1->data; 
    n1->data = n2->data; 
    n2->data = temp; 
}

int minV(int& n1, int& n2)
{
	return (n1 < n2 ? n1 : n2); 
}

int maxV(int& n1, int& n2)
{
	return (n1 > n2 ? n1 : n2); 
}
