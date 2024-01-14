#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <chrono>


using namespace std;

void mat_mul_transpose(int size,vector<vector<int>> mat1, vector<vector<int>> mat2_transpose, vector<vector<int>> result){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            result[i][j] = mat1[i][j] * mat2_transpose[i][j];
        }
    }
}

void mat_mul(int size,vector<vector<int>> mat1, vector<vector<int>> mat2, vector<vector<int>> result){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            result[i][j] = mat1[i][j] * mat2[j][i];
        }
    }
}

void init_mat(int size,vector<vector<int>> mat){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            mat[i][j] = i+j;
        }
    }
}

int main(int argc, char* argv[]){
    int lower = 1;
    int upper = 1000;
    ofstream myfile;
    myfile.open ("results.csv");
    myfile << "matrix size,matrix multiplication time,matrix multiplication time using a transpose matrix\n";
    for (int i = lower; i <= upper; i++){
        int size = i;
        vector<vector<int>> A(size, vector<int>(size));
        vector<vector<int>> B(size, vector<int>(size));
        vector<vector<int>> C(size, vector<int>(size));
        init_mat(size,A);
        init_mat(size,B);
        auto start = chrono::system_clock::now();
        mat_mul(size,A,B,C);
        auto end = chrono::system_clock::now();
        chrono::duration<double> time = (end - start);

        auto start_transpose = chrono::system_clock::now();
        mat_mul_transpose(size,A,B,C);
        auto end_transpose = chrono::system_clock::now();
        chrono::duration<double> time_transpose = (end_transpose - start_transpose);

        myfile << i << "," << time.count() << "," << time_transpose.count() <<"\n";
        //cout << "time to finish: " << time.count() << " seconds" << endl;
    }
    cout << "done" << endl; 
    myfile.close();
    return 0;
} 
