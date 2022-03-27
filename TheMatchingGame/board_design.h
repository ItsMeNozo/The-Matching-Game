#pragma once
#include "utility.h"

const int startX = 5, startY = 2; //starting position, we draw the board from here
const int rowDist = 4; 

void printHorizontalLine(int n, int r, int l, int w, bool isLower) //r for row, l: len, w: wid of the cursor
{
    
    for (int i = 0; i < n; i++)
    {
        if ((i == w) && ((r == l) || (r == l + 1 && !isLower))) //r == l: set that row
        {
            colorText(FOREGROUND_BLUE);
            std::cout << " -------";
            colorText(7); //back to white
        }
        else std::cout << " -------";
    }
    std::cout << std::endl;

}

void printVerticalLine(int n, int r, int l, int w)
{
    for (int i = 0; i < n; i++)
    {
        if ((r == l) && (i == w || (i == w - 1)))
        {
            colorText(FOREGROUND_BLUE);
            std::cout << "       |";
            colorText(7);
        }
        else std::cout << "       |";

    }
    std::cout << std::endl;
}

void printMostLeftVerticalLine(int i, int curRow, int wid)
{
    if (i == curRow && wid == 0)
    {
        colorText(FOREGROUND_BLUE);
        std::cout << "|";
        colorText(7);
    }
    else
        std::cout << "|";
}

void printIcons(list2D& B, int i, int j, int color)
{
    if (B.getNode(i, j)->data != 0)
        colorText(color); 
    std::cout << (char)B.getNode(i, j)->data;
    colorText(7);
}

void printBoard(list2D& B, int color)
{

    int i, space;
    int curRow = B.cursor / B.colSize; //y coordinates of the cursor
    int wid = B.cursor % B.colSize; //x 
    
     
    for (i = 0; i < B.rowSize; i++)
    {
        int hLineStartY = startY + rowDist * i + 1; 

        gotoxy(startX, startY + rowDist * i);
        printHorizontalLine(B.colSize, i, curRow, wid, 0);
        //first row
        gotoxy(startX, hLineStartY); 
        printMostLeftVerticalLine(i, curRow, wid);
        printVerticalLine(B.colSize, i, curRow, wid);
        //second row
        gotoxy(startX, ++hLineStartY);
        printMostLeftVerticalLine(i, curRow, wid);

        for (int j = 0; j < B.colSize; j++)
        {
            if (B.getNode(i, j)->data != 0)
            {
                printSpaces(3); 
                printIcons(B, i, j, color); 
                printSpaces(3);
                //print the rightmost |
                if ((i == curRow) && (j == wid || (j == wid - 1)))
                {
                    colorText(color);
                    std::cout << "|";
                    colorText(7);
                }
                else std::cout << "|";
            }
            else {
                if ((i == curRow) && (j == wid || (j == wid - 1)))
                {
                    colorText(color);
                    std::cout << "       |";
                    colorText(7);
                }
                else std::cout << "       |";
            }
        }
        std::cout << "\n";
        //third row
        gotoxy(startX, ++hLineStartY);
        printMostLeftVerticalLine(i, curRow, wid);
        printVerticalLine(B.colSize, i, curRow, wid);

    }
    gotoxy(startX, startY + rowDist * B.rowSize);
    printHorizontalLine(B.colSize, B.colSize - 1, curRow, wid, 1);
}
void shuffleMatrix(int matrix[][MAX], int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            int k = rand() % row;
            int l = rand() % col;
            swapVals(&matrix[i][j], &matrix[k][l]);
        }
    }
}
//shuffle when the player has no more moves. 
void shuffleLL(list2D& board)
{
    for (int i = 0; i < board.rowSize; ++i)
    {
        for (int j = 0; j < board.colSize; ++j)
        {
            int k = rand() % board.rowSize;
            int l = rand() % board.colSize;
            swapLLVals(board.getNode(i, j), board.getNode(k, l));
        }
    }
}
void generateCharMatrix(int matrix[][MAX], int row, int col)
{
    srand(time(NULL));
    int maxDistinceCharNum = (row * col) / 2;
    //generate distinct character numbers from max/1.25 -  to max
    int distinctCharNum = rand() % ((maxDistinceCharNum - maxDistinceCharNum / 2) + 1) + (int)ceil((float)maxDistinceCharNum / 1.25);

    //let the number of pairs for each character be 1 to 2
    //generate an array of the number of occurences for each character
    list1DChar charOccurrenceList;

    int sum = 0;
    for (int i = 0; i < distinctCharNum; ++i)
    {
        int pairs = rand() % 2 + 1;
        charOccurrenceList.push_back(rand() % 26 + 65, pairs * 2);
        sum += pairs * 2;
    }
    //if sum > row * col, there wont be any problems so we only consider sum < row * col
    if (sum < row * col)
    {
        Node1DChar* last = charOccurrenceList.getTail();
        int occurNum = last->singleChar.numberOfOccurences;
        last->singleChar.numberOfOccurences = occurNum + (row * col - sum);
    }
    //assign char
    Node1DChar* headptr = charOccurrenceList.getHead();

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            matrix[i][j] = headptr->singleChar.ASCIIcode;
            --headptr->singleChar.numberOfOccurences;
            //go to the next character
            if (headptr->singleChar.numberOfOccurences == 0)
                headptr = headptr->next;
        }
    }
    //shuffle the matrix
    shuffleMatrix(matrix, row, col);

}
void constructLLBoard(int charMatrix[][MAX], list2D& charBoard)
{
    for (int i = 0; i < charBoard.rowSize; ++i)
    {
        for (int j = 0; j < charBoard.colSize; ++j)
        {
            charBoard.push_back(i, j, charMatrix[i][j]);
        }
    }
}