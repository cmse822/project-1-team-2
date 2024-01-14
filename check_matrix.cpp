#include <iostream>
using namespace std;

int main(){
    int row_1, row_2, col_1, col_2, i, j, k;

    // enter rows and columns for matrix 1
    cout << "Enter rows and columns for matrix 1: ";
    cin >> row_1 >> col_1;

    // enter rows and columns for matrix 2
    cout << "Enter rows and columns for matrix 2: ";
    cin >> row_2 >> col_2;

    // check to see if matrix meets multiplication condition
    if (col_1 != row_2)
    {
        cout << "Matrices dimension do not support matrix multiplication";
        return 0;
    }
    


}