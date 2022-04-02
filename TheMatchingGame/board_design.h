#pragma once
#include "utility.h"

#define defaultColor 38 //lime green
#define highlightColor 28 //yellow
const int startX = 4, startY = 2; //starting position, we draw the board from here
const int cellH = 4, cellW = 8; 
void constructLLBoard(Node1DChar*, list2D&);
void printACell(list2D&, int, int, int); 

void printBackground()
{
    //print another art
    gotoxy(startX, startY); 
    std::cout << "                                  ,'\\"; 
    gotoxy(startX, startY + 1);
    std::cout << "    _.----.        ____         ,'  _\\   ___    ___     ____"; 
    gotoxy(startX, startY + 2);
    std::cout << "_,-'       `.     |    |  /`.   \\,-'    |   \\  /   |   |    \\  |`.";
    gotoxy(startX, startY + 3);
    std::cout << "\\      __    \\    '-.  | /   `.  ___    |    \\/    |   '-.   \\ |  |";
    gotoxy(startX, startY + 4);
    std::cout << " \\.    \\ \\   |  __  |  |/    ,','_  `.  |          | __  |    \\|  |";
    gotoxy(startX, startY + 5);
    std::cout << "   \\    \\/   /,' _`.|      ,' / / / /   |          ,' _`.|     |  |";
    gotoxy(startX, startY + 6);
    std::cout << "    \\     ,-'/  /   \\    ,'   | \\/ / ,`.|         /  /   \\  |     |";
    gotoxy(startX, startY + 7);
    std::cout << "     \\    \\ |   \\_/  |   `-.  \\    `'  /|  |    ||   \\_/  | |\\    |";
    gotoxy(startX, startY + 8);
    std::cout << "      \\    \\ \\      /       `-.`.___,-' |  |\\  /| \\      /  | |   |";
    gotoxy(startX, startY + 9);
    std::cout << "       \\    \\ `.__,'|  |`-._    `|      |__| \\/ |  `.__,'|  | |   |";
    gotoxy(startX, startY + 10);
    std::cout << "        \\_.-'       |__|    `-._ |              '-.|     '-.| |   |";
    gotoxy(startX, startY + 11);
    std::cout << "                                `'                            '-._|";

}
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
        int y = (third ? startY + 3 : startY + 1) + (r - 1)* cellH; 
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


void printACell(list2D& B, int row, int col, int color, bool rewrite)
{
    //cursor position
    int curRow = B.cursor / (B.colSize + 2); //y coordinates of the cursor
    int curCol = B.cursor % (B.colSize + 2); //x 
    //cell position for gotoxy
    int x = startX + (col - 1) * cellW; 
    int y = startY + (row - 1) * cellH; 
    //if the cursor is not there, reset color
    if ((curRow != row || curCol != col) && !rewrite)
        color = 7; 

    //1st line
    gotoxy(x, y);
    
    std::cout << " -------";
     
    //2nd, 3rd and 4th lines
    for (int i = 1; i <= 3; ++i)
    {
        gotoxy(x, y + i);
        if (i == 2)
        {
            std::cout << "|";
            colorText(color);
            //print icon
            printSpaces(3);
            std::cout << (char)B.getNode(row, col)->data; //error
            printSpaces(3);
            //
            colorText(7); 
            std::cout << "|";
        }
        else
        {
            std::cout << "|"; 
            colorText(color);
            std::cout << "       "; 
            colorText(7);
            std::cout << "|"; 
        }
            
    }
    //last line
    gotoxy(x, y + 4);
    std::cout << " -------";
}
void printBoard(list2D& B, int color, bool rewrite, Point& rewriteP)
{
    int space = 0; 
    int maxCol = 0; 
    for (int i = 1; i <= B.rowSize; i++)
    {
        Node2D* temp = B.heads[i]->right;
        int cnt = 1;

        while (temp != B.tails[i] && temp)
        {
            printACell(B, i, cnt, color, 0);
            temp = temp->right; //error
            if (i == rewriteP.x)
                maxCol = cnt;
            ++cnt;
            
        }
    }

    //keep the last selected highlighted before choosing the 2nd one
    if (rewrite && rewriteP.y <= maxCol)
        printACell(B, rewriteP.x, rewriteP.y, highlightColor, rewrite); 
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
}