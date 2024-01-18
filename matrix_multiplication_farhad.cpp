#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <random>

using namespace std;


void mat_mul_transpose(int size, vector<vector<double>> mat1, vector<vector<double>> mat2_transpose, vector<vector<double>> result) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            result[i][j] = mat1[i][j] * mat2_transpose[i][j];
        }
    }
}

void mat_mul(int size, vector<vector<double>> mat1, vector<vector<double>> mat2, vector<vector<double>> result) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
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

int main(int argc, char* argv[]) {
    // First we need some datapoints in between the lower and upper bounds for matrix size.
    int NumDataPoints = 15;
    double LowerExponent = 1;       // Corresponds to 10^0 --> matrix size of 1x1.
    double UpperExponent = 4.3;       // Corresponds to 10^7 --> matrix size of 10,000,000
    vector<int> MatSize(NumDataPoints);
    for (int i = 0; i < NumDataPoints; i++) {
        double Exponent = LowerExponent + i * (UpperExponent - LowerExponent) / (NumDataPoints - 1);
        MatSize[i] = pow(10, Exponent);
    }
    // Rest of the code is the same.
    ofstream myfile;
    myfile.open ("results_sfa.csv");
    myfile << "matrix size,matrix multiplication time,matrix multiplication time using a transpose matrix\n";
    for (int i : MatSize) {
        int size = i;
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


        myfile << i << "," << time.count() << "," << time_transpose.count() <<"\n";
        //cout << "time to finish: " << time.count() << " seconds" << endl;
        cout << "Mat size " << size << "; Time to finish: " << time.count() << ", " << time_transpose.count() << endl;
    }
    cout << "done" << endl; 
    myfile.close();
    return 0;
} 
