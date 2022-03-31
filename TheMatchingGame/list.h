#pragma once
#define MAX 14

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
    //initialization list for a class
    Node2D(int val) :data(val), right(NULL) {};
};
struct list2D {
    //create a list of {row number} heads and tails
    int rowSize, colSize;
    int cursor; 
    //assign heads and tails to NULL
    Node2D* heads[MAX] = { 0 };
    Node2D* tails[MAX] = { 0 };

    list2D(int row, int col) : rowSize(row), colSize(col) {};

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

        if (heads[row] == NULL)
            heads[row] = tails[row] = node; 
        else {
            //go to the node before the destination
            tails[row]->right = node; 
            tails[row] = node; 
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