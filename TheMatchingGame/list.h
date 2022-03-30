#pragma once
#define MAX 30

struct Point {
    int x;
    int y;
    Point(int row, int col) :x(row), y(col) {};
};
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
        //if temp is NULL, we stop
        while (pos-- && temp)
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
    int cursor; 
    Node2D* heads[MAX] = { 0 };//just to save the addresses 
    Node2D* tailsRow[MAX] = { 0 };
    Node2D* tailsCol[MAX] = { 0 };

    list2D(int row, int col) : rowSize(row), colSize(col) {};
    Node2D* getHead(int row)
    {
        return heads[row];
    }

    Node2D* getNode(int row, int col)
    {
        Node2D* temp = heads[row]; 
        while(col--)
        {
            temp = temp->right; //error
        }
        return temp; 
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
        for (int i = 0; i <= rowSize + 1; ++i)
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