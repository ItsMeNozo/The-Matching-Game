#include <iostream>
#include <cstdlib>
#include <ctime> //srand(time(NULL))
#include <cmath>
#include <Windows.h>
#include <conio.h>
#include "list.h"
#include "board_design.h"
#include "gameplay.h"
using namespace std; 

#define MAX 14
#define defaultColor 37
#define highlightColor 28 

int main()
{
    int row, col; 
    colorText(7); 
    
    
    char c; 
    do {
        cout << "Input row and column (both numbers are not odd numbers and must be <= 10): ";
        cin >> row >> col;
        while ((row % 2 == 1 && col % 2 == 1) || (row > 10 || col > 10))
        {
            cout << "Please re-enter row and column: ";
            cin >> row >> col;
        }
        list2D charBoard(row, col);
        generateCharMatrix(charBoard, row, col);
        //game instruction
        gotoxy(80, 5);
        cout << "W A S D to move around.";
        gotoxy(80, 6); 
        cout << "Press Enter to select cell";
        playGame(charBoard, defaultColor);
        
        cout << " Game replay? Type Y or N\n";
        gotoxy(startX + 2, startX); 
        cin >> c; 
    } while (c == 'Y' || c == 'y');
    
}

