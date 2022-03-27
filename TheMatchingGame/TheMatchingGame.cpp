#include <iostream>
#include <cstdlib>
#include <ctime> //srand(time(NULL))
#include <cmath>
#include <Windows.h>
#include <conio.h>
#include "list.h"
#include "board_design.h"
using namespace std; 

#define MAX 30


void printBoard(list2D& charBoard)
{
    /*for (int i = 0; i < charBoard.rowSize; ++i)
    {
        Node2D* rPtr = charBoard.getHead(i);
        while (rPtr)
        {
            cout << (char)rPtr->data << " ";
            rPtr = rPtr->right;
        }
        cout << "\n";
    }*/

}
int main()
{
    int row, col; 
    int charMatrix[MAX][MAX] = {
        {0,0}, 
        {0,0}
    };
    
    cout << "Input row and column (both numbers are not odd numbers): "; 
    cin >> row >> col; 
    while (row % 2 == 1 && col % 2 == 1)
    {
        cout << "Please re-enter row and column: "; 
        cin >> row >> col; 
    }
    generateCharMatrix(charMatrix, row, col); 
    list2D charBoard(row, col); 
    constructLLBoard(charMatrix, charBoard); 
    
    printBoard(charBoard); 
    
}

