#pragma once
#include "utility.h"

const int startX = 4, startY = 2; //starting position, we draw the board from here
const int rowDist = 4; 
void constructLLBoard(Node1DChar* headptr, list2D& charBoard);

void printHorizontalLine(int n, int r, int l, int w, bool isLower, int color) //r for row, l: curRow of cursor, w: curCol of the cursor
{
    for (int i = 1; i <= n; i++)
    {
        if ((i == w) && ((r == l) || (r == l + 1 && !isLower))) //r == l: set that row
        {
            colorText(color);
            std::cout << " -------";
            colorText(7); //back to white
        }
        else std::cout << " -------";
    }
}

void printVerticalLine(int n, int r, int l, int w, int color, bool third)//r for row, l: curRow of cursor, w: curCol of the cursor
{
    //8 spaces including |
    for (int i = 1; i <= n; i++)
    {
        int x = (startX + 1) + 8 * (i - 1); //8 is the width 
        int y = (third ? startY + 3 : startY + 1) + (r - 1)* rowDist; 
       /* int y = (startY + 1); 
        if (third)
            y = startY + 3; */
        gotoxy(x, y); 
        if ((r == l) && (i == w || (i == w - 1)))
        {
            colorText(color);
            std::cout << "       |";
            colorText(7);
        }
        else std::cout << "       |";

    }
}

void printMostLeftVerticalLine(int i, int curRow, int curCol, int color)
{
    if (i == curRow && curCol == 1)
    {
        colorText(color);
        std::cout << "|";
        colorText(7);
    }
    else
        std::cout << "|";
}

void printIcon(list2D &B, int i, int j)
{
    printSpaces(3);
    std::cout << (char)B.getNode(i, j)->data;
    printSpaces(3);
}

void rewriteLastSelected(list2D& B, int color, Point& rewriteP)
{
    int x = startX + (rewriteP.y-1) * (rowDist + 4);//each cell has width of 7
    int y = startY + (rewriteP.x-1) * (rowDist); 

    //1st line
    gotoxy(x, y); 
    colorText(color);
    std::cout << " -------";
    colorText(7); //back to white
    //2nd, 3rd and 4th lines
    for (int i = 1; i <= 3; ++i)
    {
        gotoxy(x, y + i);
        colorText(color);
        if (i == 2)
        {
            std::cout << "|";
            printIcon(B, rewriteP.x, rewriteP.y); 
            std::cout << "|";
        }
        else 
            std::cout << "|       |";
        colorText(7);
    }
    //last line
    gotoxy(x, y + 4);
    colorText(color);
    std::cout << " -------";
    colorText(7); //back to white
    

}
void printBoard(list2D& B, int color, bool rewrite, Point& rewriteP)
{
    int i, space;
    int curRow = B.cursor / (B.colSize + 2) ; //y coordinates of the cursor
    int curCol = B.cursor % (B.colSize + 2) ; //x 
    int hLineStartY = startY; //use this to print the start of every row
    bool emptycell = 0; 

    colorText(7); 
    for (i = 1; i <= B.rowSize; i++)
    {
        gotoxy(startX, hLineStartY);
        printHorizontalLine(B.colSize, i, curRow, curCol, 0, color);
        //first row
        gotoxy(startX, hLineStartY+ 1); 
        printMostLeftVerticalLine(i, curRow, curCol, color);
        printVerticalLine(B.colSize, i, curRow, curCol, color, 0);
        //second row
        gotoxy(startX, hLineStartY+ 2);
        printMostLeftVerticalLine(i, curRow, curCol, color);

        for (int j = 1; j <= B.colSize; j++)
        {
            if (B.getNode(i, j)->data != 0)
            {
                printIcon(B, i, j);
                //print the rightmost |
                if ((i == curRow) && (j == curCol || (j == curCol - 1)))
                {
                    colorText(color);

                    std::cout << "|";
                    colorText(7);
                }
                else
                    std::cout << "|";
            }
            else {
                if ((i == curRow) && (j == curCol || (j == curCol - 1)))
                {
                    colorText(color);
                    std::cout << "       |";
                    colorText(7);
                }
                else std::cout << "       |";
            }
        }
        //std::cout << "\n";
        //third row
        gotoxy(startX, hLineStartY+ 3);
        printMostLeftVerticalLine(i, curRow, curCol, color);
        printVerticalLine(B.colSize, i, curRow, curCol, color, 1);

        hLineStartY += 4; 

    }
    gotoxy(startX, startY + rowDist * B.rowSize);
    printHorizontalLine(B.colSize, B.rowSize, curRow, curCol, 1, color);
    //rewuite on the last selected cell
    if (rewrite)
        rewriteLastSelected(B, color, rewriteP); 
}

//shuffle when the player has no more moves. 
void shuffleLL(list2D& board)
{
    for (int i = 1; i <= board.rowSize; ++i)
    {
        for (int j = 1; j <= board.colSize; ++j)
        {
            int k = rand() % board.rowSize + 1;
            int l = rand() % board.colSize + 1;
            swapLLVals(board.getNode(i, j), board.getNode(k, l));
        }
    }
}
void generateCharMatrix(list2D& charBoard, int row, int col)
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

    constructLLBoard(headptr, charBoard); 
    //shuffle linked list
    shuffleLL(charBoard); 

}
void constructLLBoard(Node1DChar* headptr, list2D& charBoard)
{
    //we have invisible surrounding squares with value 0
    for (int i = 0; i <= charBoard.colSize + 1; ++i)
        charBoard.push_back(0, i, 0);

    for (int i = 1; i <= charBoard.rowSize; ++i)
    {
        charBoard.push_back(i, 0, 0); 
        for (int j = 1; j <= charBoard.colSize; ++j)
        {
            charBoard.push_back(i, j, headptr->singleChar.ASCIIcode);
            --headptr->singleChar.numberOfOccurences;
            //go to the next character
            if (headptr->singleChar.numberOfOccurences == 0)
                headptr = headptr->next;
        }
        charBoard.push_back(i, charBoard.colSize + 1, 0); 
    }

    for (int i = 0; i <= charBoard.colSize + 1; ++i)
        charBoard.push_back(charBoard.rowSize + 1, i, 0);

    //connect rows 
    Node2D* n1, *n2; 
    for (int i = 0; i <= charBoard.rowSize; ++i)
    {
        n1 = charBoard.getHead(i);
        n2 = charBoard.getHead(i + 1);

        while (n1 && n2)
        {
            n1->down = n2;
            n1 = n1->right; 
            n2 = n2->right; 
        }
    }
    
}