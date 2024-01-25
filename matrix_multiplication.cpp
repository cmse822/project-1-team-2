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
            for(int k = 0; k < size; k++){
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

void mat_mul_transpose(int size,vector<vector<double>> mat1, vector<vector<double>> mat2, vector<vector<double>> result){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            for(int k = 0; k < size; k++){
                result[i][j] += mat1[i][k] * mat2[j][k];
            }
        }
    }
}

void init_mat(int size, vector<vector<double>> mat) {
    // Uniform distribution [-1,1]
    random_device rd;                           // Hardware random number
    mt19937 eng(rd());                          // Seed the generator
    uniform_real_distribution<> distr(-1, 1);   // Define the range

    // Populate matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            mat[i][j] = distr(eng);
        }
    }
}

void init_mat_zeros(int size, vector<vector<double>> mat){
    // Populate matrix
    for (int i =0; i < size; i++){
        for (int j = 0; j < size; j++){
            mat[i][j] = 0;
        }
    }
}

void batch_experiment(int mat_size,int batch_size,ofstream& myfile){
    double sum = 0;
    double mean = 0;
    double std = 0;
    double sum_t = 0;
    double mean_t = 0;
    double std_t = 0;

    vector<double> times(batch_size);   
    vector<double> times_t(batch_size);       
    for (int batch_iter = 0; batch_iter < batch_size; batch_iter++){
        // Instantiate matrices
        vector<vector<double>> A(mat_size, vector<double>(mat_size));
        vector<vector<double>> B(mat_size, vector<double>(mat_size));
        vector<vector<double>> C(mat_size, vector<double>(mat_size));

        // Initialize matrices
        init_mat(mat_size,A);
        init_mat(mat_size,B);
        init_mat_zeros(mat_size,C);

        // Execute and time matrix multiplication
        auto start = chrono::system_clock::now();
        mat_mul(mat_size,A,B,C);
        auto end = chrono::system_clock::now();
        chrono::duration<double> time = (end - start);

        auto start_t = chrono::system_clock::now();
        mat_mul_transpose(mat_size,A,B,C);
        auto end_t = chrono::system_clock::now();
        chrono::duration<double> time_t = (end - start);

        // Calculate and store time
        sum += time.count();
        times[batch_iter] = time.count();
        
        sum_t += time_t.count();
        times_t[batch_iter] = time_t.count();
    }

    // Calculate time average
    mean = sum/batch_size;
    mean_t = sum_t/batch_size;

    // Calculate standard deviation
    sum = 0;
    sum_t = 0;
    for (int batch_iter = 0; batch_iter < batch_size; batch_iter++){
        sum += pow(times[batch_iter] - mean,2);    
        sum_t += pow(times_t[batch_iter] - mean_t,2);
    }
    std = sqrt(sum/batch_size);
    std_t = sqrt(sum_t/batch_size);

    // Write to file
    myfile << mat_size << "," << mean << "," << std << "," << mean_t << "," << std_t << "," << "\n";
    cout << mat_size << endl;
}

int main(int argc, char* argv[]){
    int num_steps = 10;
    double power_low = 1;
    double power_high = 3.7;
    double power_step = (power_high - power_low) / num_steps;
    int batch_size = 7;

    // Open file
    ofstream myfile;
    myfile.open ("results.csv");
    myfile << "matrix size, average time, standard deviation, average time transpose, standard deviation" << endl;
    
    // Run experiment with different matrix sizes
    for (int i = 0 ; i <= num_steps; i++){
        // Calculate matrix power
        double power = power_low + (i * power_step);

        // Calculate matrix size 
        int size = static_cast<int>(pow(10,power));

        // Perform experiment
        cout << "Performing experiment for p=" << power << " (N=" << size << ")" << endl;
        batch_experiment(size,batch_size,myfile);
    }
    cout << "done" << endl; 
    myfile.close();
    return 0;
} 
