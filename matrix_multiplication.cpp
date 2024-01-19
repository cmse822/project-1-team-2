#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <random>
#include <cmath>

using namespace std;

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

void batch_experiment(int mat_size,int batch_size,ofstream& myfile){
    double sum = 0;
    double mean = 0;
    double std = 0;

    vector<double> times(batch_size);        
    for (int batch_iter = 0; batch_iter < batch_size; batch_iter++){
        // Instantiate matrices
        vector<vector<double>> A(mat_size, vector<double>(mat_size));
        vector<vector<double>> B(mat_size, vector<double>(mat_size));
        vector<vector<double>> C(mat_size, vector<double>(mat_size));

        // Initialize matrices
        init_mat(mat_size,A);
        init_mat(mat_size,B);

        // Execute and time matrix multiplication
        auto start = chrono::system_clock::now();
        mat_mul(mat_size,A,B,C);
        auto end = chrono::system_clock::now();
        chrono::duration<double> time = (end - start);

        // Calculate and store time
        sum += time.count();
        times[batch_iter] = time.count();
    }

    // Calculate time average
    mean = sum/batch_size;

    // Calculate standard deviation
    sum = 0;
    for (int batch_iter = 0; batch_iter < batch_size; batch_iter++){
        sum += pow(times[batch_iter] - mean,2);
    }
    std = sqrt(sum/batch_size);

    // Write to file
    myfile << mat_size << "," << mean << "," << std << "," << "\n";
    cout << mat_size << endl;
}

int main(int argc, char* argv[]){
    int lower = 1000;
    int upper = 10000;
    int step = 1000;
    int batch_size = 10;
    int size = 0;

    ofstream myfile;
    myfile.open ("results.csv");
    myfile << "matrix size,M.M. average time,M.M.T. average time,M.M. std,M.M.T. std\n";
    // run a matrix multiplication experiment with size 100 with 'batch_size' number of iterations 
    batch_experiment(100,batch_size,myfile);
    for (int i = lower; i <= upper; i += step){
        size = i;
        // run a matrix multiplication experiment with size 'i' with 'batch_size' number of iterations 
        batch_experiment(i,batch_size,myfile);
    }
    cout << "done" << endl; 
    myfile.close();
    return 0;
} 
