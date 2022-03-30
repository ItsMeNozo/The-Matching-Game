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

#define MAX 30


int main()
{
    int row, col; 
    int color = 9; 
    cout << "Input row and column (both numbers are not odd numbers): "; 
    cin >> row >> col; 
    while (row % 2 == 1 && col % 2 == 1)
    {
        cout << "Please re-enter row and column: "; 
        cin >> row >> col; 
    }

    list2D charBoard(row, col);
    generateCharMatrix(charBoard, row, col); 
    
    moveBoardCursor(charBoard, color);
    
}

