#pragma once
#include "utility.h"


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
            matrix[i][j] = headptr->singleChar.ASCIIcode;//error
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