#pragma once
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

void gotoXY(int x, int y)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c{ x, y };
    SetConsoleCursorPosition(console, c);
}