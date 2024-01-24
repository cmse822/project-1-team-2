# Report

Team Members: Esteban Echeverri, John Salako, Farhad Abdollahi, Jorge Martinez-Ortiz.

## Warm-up Answers

The "Arithmetic Intensity" of four operations in Warm-up question are shown as follows:

Kernel | Arithmetic Intensity | FLOPs/byte
--|--|--
Y[j] += Y[j] + A[j][i] * B[i] | 3/4   | 3/32 
s += A[i] * A[i]              | 2     | 1/4
s += A[i] * B[i]              | 1     | 1/8
Y[i] = A[i] + C*B[i]          | 2/3   | 1/12

___
## Part 1: Matrix-matrix Multiplication

In the matrix multiplication $AB = C$, we have that a single column of $C$ is given by

$$c_i = \sum_{k=1}^N a_{ik}b_{ki}.$$


### Question 1:

The "matrix-matrix multiplication" code has been developed in `C++` programming language under the `matrix_multiplication.cpp`. The operation is developed as a triple-nested loop over to calculate the results under the `matmul(A, B, C)` function. In this code, 10 different matrix sizes from `N=10` to `N=1000` has been evaluated (square matrices), where the elements of each matrix were randomly generated as a `double` type between -1 and 1. The runtime of operation has been evaluated by the mean and standard deviation of 7 different runs. 

### Question 2: 

Considering the square matrix assumption and the multiplication formula presented above, for a given matrix size `N`, the following floating point operations are required: 

- Vector product: $N$ multiplications and $N-1$ additions.
- Each column of $C$ has $N$ entries.
- $C$ has $N$ columns.

Therefore, the total number of operations requires is $N^3-N^2=O(N^3)$.

### Question 3: 

In our code, the runtime of the operation is measured through the `chrono::system_clock::now()` by measuring the system clock time before and after the operation. The total number of floating point operations are also estimated using the $N^3-N^2$ (based on Question 2), which is 0.99 Mflop. The following table shows the results for matrix size of `N=100`.

User | Architecture | Avg Runtime (ms) | Mflops/s
--|--|--|--|
Esteban  | local        | 7.747  | 127.791
Esteban  | amd20        | 16.469 | 60.113
John     | local        | 13.895 | 71.248
John     | intel18      | 15.282 | 64.782
Jorge    | local        | 10.108 | 97.942
Jorge    | amd20-v100   | 16.159 | 61.266
Farhad   | local        | 11.389 | 86.826
Farhad   | intel16      | 14.476 | 68.389

### Question 4:


As mentioned earlier, each groupmember uses a local and HPCC machines for running the "matrix-matrix multiplication" code at 10 different matrix sizes; where the architecture of the machines are described in the following table.

`NOTE`: the problem statement asks to use the assumption of "1 flop per cycle" for calculation of the "Theoretical Peak Performance" (TPP), this is while it's not the case for almost every CPU architecture nowadays. According to online search, the assumption of 8 flops/cycle for x86 and 16 flops/cycle for x64 has also be used.

User | Architecture | Clock speed (GHz) | Cache Size | Cache Layout | Cores | TPP (GFlops/s) - Assmp: 1 flop/cycle | Avg Runtime (ms) $N=100$| Mflops/s $N=100$ |
--|--|--|--|--|--|--|--|--|
Esteban | local | 2.3 | 480 KB, 320 KB, 12.5 MB, 24 MB | 10 L1d, 10 L1i, 10 L2, 1 L3   | 10  | 23.0 | 7.7.747 | 127.791  
Esteban | amd20 | 2.6 | 32 KB, 32 KB, 512 KB, 16.38 MB | 64 L1d, 64 L1i, 64 L2, 1 L3   | 64  | 166.4  | 16.469 | 60.113
John    | local | 2.6 | 384KB, 1.5MB, 12MB |6 L1, 6 L2, 1 L3 | 6 | 15.6 | 13.895 | 71.248 
John    | intel18 | 2.4 | 32K, 32KB, 1MB, 28.160MB |20 L1d, 20 L1i, 20 L2, 1 L3 | 20 | 48.0 | 15.282 | 64.782          
Jorge   | local (Apple Silicon M2) | 3.2 | 192KB, 12MB, 8MB | 8 L1, 8 L2, 1 L3| 8 | 25.6 | 10.108 | 97.942
Jorge   | amd20-v100 | 2.4 | 32 KB, 32 KB, 1024 KB, 36.60MB | 64 L1d, 64 L1i, 64 L2, 1 L3 | 48 | 115.2 | 16.159 | 61.266
Farhad  | local | 2.6 | 64KB, 256KB, 12MB | 6 L1, 6 L2, 1 L3 | 6 | 15.6 | 11.389 | 86.826
Farhad  | intel16 | 2.4 | 32KB, 256KB, 35MB | 14 L1, 14 L2, 1 L3 | 14 | 33.6 | 14.476 | 68.389

Finally, the Mflop/s of the matrix multiplication of size `N=100` is plotted against the Theoretical Peak Performance (Gflops/s) of the available CPUs for local and HPCC machine types. As this figure shows for the local machines, the rate of floating point operations (Mflops/s) is generally increased with an increase in the CPU computational capacity (TPP). However, for the HPCC machines, this trend is unexpectedly reversed; where the rate of the floating point operations decreases with an increase in TPP, especially for the more recent `amd20` machine which has 64 cores.

![img1](/analysis/Part1_Q4_Mflops_vs_TPP.png)


### Question 5:

The following figure shows the results for different matrix sizes from `N=1` to `N=1000`; where the horizontal axis shows the number of floating point operations ($N^3-N^2$) and vertical axis represents the average rate of the floating point operations. As this figure shows for both locan and HPCC runs, the rate of the floating point operation increases with an increase in the matrix size and reaches to a peak value. This peak value represents the ridge-point at which the bandwidth-bound and computing-bounds conflicted together. After this point, the matrix size is too large for the Cash replacements and therefore the rate of floating point operations reduces, due to the unavailability of the data in high-level memory cashes.

#### Discussion based on system architecture:

For instance in the local runs, the highest performance is for Esteban's systems which have the highest L1 cash capacity and common clock speed of 2.3 GHz. On the other hand, the lowest performance is for Farhad's system which has the lowest L1 cash capacity and clock speed of 2.4 GHz. Another interesting observation was that although John's system has slightly higher L1 cash capacity than that of Jorge, it's performance was a little lower. This can attribute to the considerably higher clock speed of the Jorge's system (3.2 GHz) which was about 20% higher.  

![img2](/analysis/Part1_Q5_Mflops-s_vs_Mflops.png)

As requested in Question 5, the above-mentioned figure is re-generated as follows in log-log space with the horizontal lines representing the theoritical peak performance (TPP) of the hardware. As this figure shows, the TPP caps of the analysis were far beyond the measured performance. The main reason for this observation can attribute the number of cores within the CPU. In another word, the number of cores is a direct multiplier in calculation of the TPP; this is while most of those cores are not contribute in the performing the analysis for the code (the task is suppose to be single-core). Therefore, the full capacity of the system is not expected to be used during the matrix multiplication process. Such a gap seems even more significant in HPCC, where there are considerably more number of cores available for each CPU. 

![img10](/analysis/Part1_Q5_With_TPP.png)

### Question 6:

According to the figure, it was an interesting observation that the peak performance point of all graphs happens at `Mflops=3.91`, which corresponds to the matrix size of about `N=160`, the most likely reason for this behavior is that the memory usage for this problem increases exponentially and the difference between L1 cache sizes between systems not being significantly high, then we can assume that for most of them a matrix with `N=160` is the one that fit the best on the L1 cache.

## Part 2: The Roofline Model

The CS Roofline Toolkit 1.1.0 has been cloned and run by each groupmember on the same computing systems used for Part 1. In what follows, the project questions are addressed:

### Question 1& 2:

After cloning the Roofline Toolkit from GitHub, each groupmember modify the `config` file corresponds to the system used for running. The main modifications were disabling the OpenMM and MPI parts, changing the compiler to the gcc system compiler, removing the compiler options, and specify the output folders. 

### Question 3:

Based on the ERT Toolkit model results, the peak performance and bandwidths for different cash levels can be summarized in the following table:

User | Architecture | Peak Performance (Gflops/s) | L1 Cash (GB/s) | L2 Cash (GB/s) | DRAM (GB/s)
--|--|--|--|--|--|
Esteban | local      | 24.11 | 144.18 | 88.79 | 25.14 
Esteban | amd20      | 13.42 | 73.26  | N/A | 25.66 
John    | local      | 16.37 | 130.13 | 104.41 | 17.32 
John    | intel18    | 14.62 | 111.83 | 73.18 | 19.31 
Jorge   | local      | 32.5 | 99.4 | 66.7 | 43.3 
Jorge   | amd20-v100 | 13.4 | 46.0 | N/A | 20.3 
Farhad  | local      | 14.59 | 67.41  | 49.00 | 21.25 
Farhad  | intel16    | 11.96 | 86.47  | 68.15 | 16.77 
*N/A: Not Available

The resulting Roofline models for all computing systems of each groupmember can be found in the figure below. As each model in this figure shows, the horizontal cap of the model shows the maximum peak performance (comput-bound) where the inclined lines shows the limitations comes from the memory bandwidth capacity at different memory/cash level (bandwidth-bound).

![img3](/analysis/Part2_Q3_Rooflines.png)

Based on the presented results in the figure above, the "ridge point" for each system architecture and memory type can be found in the following table. It is noted that the higher the value of the ridge point indicates more kernel intensity required to achieve the compute-bound (CPU capacity).

User | Architecture | Ridge Point (Flops/Byte) | | |
-----|--------------|-------|-------|----|
 | |                 | L1 Cash | L2 Cash | DRAM| 
Esteban | local      | 0.167 | 0.271 | 0.959 |
Esteban | amd20      | X | X  | X |
John    | local      | 0.126 | 0.156 | 0.945 |
John    | intel18    | 0.130 | 0.199 | 0.757 |
Jorge   | local      | 0.327 | 0.486 | 0.751 |
Jorge   | amd20-v100 | 0.292 | N/A   | 0.625 |
Farhad  | local      | 0.216 | 0.297 | 0.686 |
Farhad  | intel16    | 0.138 | 0.175 | 0.713 |


### Question 4:
The following table illustrates the theoretical performance of four of the "seven dwarfs" kernels in the architectures tested in this project. 

| Kernel | Operational Intensity | Esteban Local | amd20 | John Local | intel18| Jorge Local | amd20-v100 | Farhad Local | intel16|
|--|--|--|--|--|--|--|--|--|--|
| Sparse Matrix-Vector Multiplication | 0.25 |  Compute | X | Compute | Compute | Memory | Memory | Compute | Compute |
| Lattice-Boltzmann Magnetohydrodynamics | 1.07 | Compute | X | Compute | Compute | Compute | Compute | Compute | Compute |
| Stencil | 0.5 | Compute | X | Compute | Compute | Compute | Compute | Compute | Compute | 
| 3-D FFT | 1.64 | Compute | X | Compute | Compute | Compute | Compute | Compute | Compute | 

In the vast majority of the benchmarked architectures, and if we consider the peak theoretical cealing of each of them that corresponds to the L1 cache, the kernels would be compute bound. 

The optimization strategies that should be implemented in each of these cases should, in principle, allow them to overcome the celings that would initially prevent them from reaching the maximum theoretical roof. For the cases where the kernels are compute bound, improving the instruction level parallelism and balancing floating-point operations would be advisable. This second strategy requires having a similar number of floating-point additions and subtractions, which is directly related to the algorithm implemented and might not always be possible to modify. On the other hand, relying on ILP may vary on the architecture, although loop unrolling might be a way to achieve it. 

### Question 5:
The same analaysis of the performance of the four kernels studied in the warm-up is shown in this table
| Kernel | Operational Intensity | Esteban Local | amd20 | John Local | intel18 | Jorge Local | amd20-v100 | Farhad Local | intel16|
|--|--|--|--|--|--|--|--|--|--|
Y[j] += Y[j] + A[j][i] * B[i] | 0.09375 | Memory | X | Memory | Memory  | Memory | Memory | Memory | Memory | 
s += A[i] * A[i]              | 0.25    | Compute | X | Compute | Compute | Memory | Memory  | Compute | Compute | 
s += A[i] * B[i]              | 0.125   | Memory | X | Memory | Memory | Memory | Memory | Memory  | Memory |
Y[i] = A[i] + C*B[i]          | 0.0833  | Memory | X | Memory | Memory | Memory | Memory | Memory | Memory | 

In contrast with the kernels previously discussed in question 4, most of the kernels of the warm-up are bound by memory. In order to guarantee maximum efficiency to overcome the ceiling on memory bound problem, restructuring loop to guarantee stride acces and relying on software prefetching are viable solutions

### Question 6:

Looking at the values of the theoretical peak performance that we got from the matrix matrix multiplication and the ones that we got from ERT show that assuming that the CPU only does one instruction per cycle is a good approximation most of the time, but the actual peaks are higher and demonstrate that modern CPUs work on more than one instruction per cycle, for example, Jorge's local environment estimate a peak performance of 25.6 GFlops/s but the peak given by ERT is 32.5, which is a significantly higher than the estimate using just one instruction per cycle.
