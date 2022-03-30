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


int main()
{
    int row, col; 
    int color = 9; 
    cout << "Input row and column (both numbers are not odd numbers and must be <= 10): "; 
    cin >> row >> col; 
    while ((row % 2 == 1 && col % 2 == 1) || (row > 10 || col > 10))
    {
        cout << "Please re-enter row and column: "; 
        cin >> row >> col; 
    }

    //game instruction
   /* gotoxy(80, 5); 
    cout << "W A S D to move around."; 
    cout << "Press Enter to select cell"; */
    list2D charBoard(row, col);
    generateCharMatrix(charBoard, row, col); 
    
    moveBoardCursor(charBoard, color);
    
}

