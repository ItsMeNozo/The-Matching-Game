#include <iostream>
using namespace std; 

#define MAX 100
struct Node {
    int data; 
    Node *right, 
        *down; 
};
Node* construct2DList(int a[][MAX], int i, int j, int row, int col)
{
    if (i > row - 1 || j > col - 1)
    {
        return NULL; 
    }
    //assign value
    Node* n = new Node; 
    n->data = a[i][j];
    //right
    n->right = construct2DList(a, i, j + 1, row, col); 
    //down
    n->down = construct2DList(a, i + 1, j, row, col);
    return n; 
}
void display(Node* head)
{
    Node* rp,
        * dp = head; 
    while (dp)
    {
        rp = dp; 
        while (rp)
        {
            cout << rp->data << " ";
            rp = rp->right; 
        }
        cout << "\n";
        dp = dp->down; 
    }
}
int main()
{
    int arr[MAX][MAX]; 
    int row = 3,
        col = 3;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            cin >> arr[i][j];
    
    Node* head = construct2DList(arr, 0, 0, row, col);
    display(head);

}


