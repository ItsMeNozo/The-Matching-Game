#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

#define MAX 30

struct Character {
    int ASCIIcode;
    int numberOfOccurences;
};


struct Node1DChar {
    Character singleChar;
    Node1DChar* next;
    Node1DChar(int ascii, int occur) : singleChar({ ascii , occur }), next(NULL) {};
};
struct list1DChar {
    Node1DChar* head;
    Node1DChar* tail;

    list1DChar() : head(NULL), tail(NULL) {};
    Node1DChar* getHead()
    {
        return head;
    }
    Node1DChar* getTail()
    {
        return tail;
    }


    void push_back(int ascii, int occur)
    {
        Node1DChar* node = new Node1DChar(ascii, occur);
        if (!head) //head == NULL
        {
            head = tail = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
    }
    ~list1DChar()
    {
        Node1DChar* temp = head;
        while (temp)
        {
            Node1DChar* temp1 = temp->next;
            delete temp;
            temp = temp1;
        }
    }
};
struct Node1D {
    int data;
    Node1D* next;
    Node1D(int val) : data(val), next(NULL) {};
};
struct list1D {
    Node1D* head;
    Node1D* tail;
    list1D() : head(NULL), tail(NULL) {};

    Node1D* getHead()
    {
        return head;
    }
    Node1D* getTail()
    {
        return tail;
    }
    Node1D* getNode(int pos)
    {
        Node1D* temp = head;
        while (pos--)
        {
            temp = temp->next;
        }
        return temp;
    }
    void push_back(int val)
    {
        Node1D* node = new Node1D(val);
        if (!head) //head == NULL
        {
            head = tail = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
    }

    ~list1D()
    {
        Node1D* temp = head;
        while (temp)
        {
            Node1D* temp1 = temp->next;
            delete temp;
            temp = temp1;
        }
    }
};

struct Node2D {
    int data;
    Node2D* right;
    Node2D* down;
    //initialization list for a class
    Node2D(int val) :data(val), right(NULL), down(NULL) {};
};
struct list2D {
    //create a list of {row number} heads and tails
    int rowSize, colSize;
    Node2D* heads[MAX] = { 0 };//just to save the addresses 
    Node2D* tailsRow[MAX] = { 0 };
    Node2D* tailsCol[MAX] = { 0 };

    list2D(int row, int col) : rowSize(row), colSize(col) {};
    Node2D* getHead(int row)
    {
        return heads[row];
    }

    void push_back(int row, int col, int val)
    {
        Node2D* node = new Node2D(val);
        if (heads[row] == NULL && heads[col] == NULL)
        {
            heads[row] = tailsRow[row] = tailsCol[col] = node;
        }
        else if (heads[row] == NULL) {
            tailsCol[col]->down = node;
            heads[row] = tailsRow[row] = tailsCol[col] = node;
        }
        else if (tailsCol[col] == NULL) {
            Node2D* temp = heads[row];
            while (--col)
                temp = temp->right;
            temp->right = node;
            tailsCol[col] = tailsRow[row] = node;
        }
        else {
            tailsRow[row]->right = node;
            tailsCol[col]->down = node;
            //update tails
            tailsRow[row] = node;
            tailsCol[col] = node;
        }
    }

    ~list2D()
    {
        for (int i = 0; i < rowSize; ++i)
        {
            Node2D* temp = heads[i];
            while (temp)
            {
                Node2D* temp1 = temp->right;
                delete temp;
                temp = temp1;
            }
        }
    }
};
void swapVals(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void shuffle(int matrix[][MAX], int row, int col)
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

void generateCharArray(int matrix[][MAX], int row, int col)
{
    srand(time(NULL));
    int maxDistinceCharNum = (row * col) / 2;
    //generate distinct character numbers from max/1.25 -  to max
    int distinctCharNum = rand() % ((maxDistinceCharNum - maxDistinceCharNum / 2) + 1) + ceil((float)maxDistinceCharNum / 1.25);

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
    //shuffle the array
    shuffle(matrix, row, col);

}
void constructBoard(int charMatrix[][MAX], list2D& charBoard, int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            charBoard.push_back(i, j, charMatrix[i][j]);
        }
    }
}
void printBoard(list2D& charBoard, int row)
{

    for (int i = 0; i < row; ++i)
    {
        Node2D* rPtr = charBoard.getHead(i);
        while (rPtr)
        {
            cout << (char)rPtr->data << " ";
            rPtr = rPtr->right;
        }
        cout << "\n";
    }
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
    constructLLBoard(charMatrix, charBoard, row, col);



    printBoard(charBoard, row);

}

