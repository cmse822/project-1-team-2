#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <random>
#include <cmath>


using namespace std;

void mat_mul_transpose(int size,vector<vector<double>> mat1, vector<vector<double>> mat2_transpose, vector<vector<double>> result){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            result[i][j] = mat1[i][j] * mat2_transpose[i][j];
        }
    }
}

void mat_mul(int size,vector<vector<double>> mat1, vector<vector<double>> mat2, vector<vector<double>> result){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            result[i][j] = mat1[i][j] * mat2[j][i];
        }
    }
}

void init_mat(int size, vector<vector<double>> mat) {
    // Random number generator, to initialize the Matrix with numbers between [-1, 1]
    random_device rd;                           // Obtain a random number from hardware
    mt19937 eng(rd());                          // Seed the generator
    uniform_real_distribution<> distr(-1, 1);   // Define the range
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mat[i][j] = distr(eng);
        }
    }
}

int main(int argc, char* argv[]){
    int lower = 100;
    int upper = 10000;
    int batch_size = 100;
    int size = 0;
    int step = 100;
    double sum = 0;
    double sum_transpose = 0;

    ofstream myfile;
    myfile.open ("results.csv");
    myfile << "matrix size,matrix multiplication time,matrix multiplication time using a transpose matrix\n";
    for (int i = lower; i <= upper; i += step){
        size = i;
        sum = 0;
        sum_transpose = 0;
        for (int batch_iter = 0; batch_iter < batch_size; batch_iter++){
            vector<vector<double>> A(size, vector<double>(size));
            vector<vector<double>> B(size, vector<double>(size));
            vector<vector<double>> C(size, vector<double>(size));
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

            //myfile << i << "," << time.count() << "," << time_transpose.count() <<"\n";
            //cout << "time to finish: " << time.count() << " seconds" << endl; 
            sum += time.count();
            sum_transpose += time_transpose.count();
        }
        myfile << i << "," << sum/batch_size << "," << sum_transpose/batch_size <<"\n";
        cout << i << endl;
    }
    cout << "done" << endl; 
    myfile.close();
    return 0;
} 
