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

User | Architecture | Clock speed (GHz) | Cache Size | Cache Layout | Cores | TPP (GFlops/s) - Assmp: 1 flop/cycle | TPP (Gflops/s) | Avg Runtime (ms) $N=100$| Mflops/s $N=100$ |
--|--|--|--|--|--|--|--|--|--|
Esteban | local | 2.3 | 480 KB, 320 KB, 12.5 MB, 24 MB | 10 L1d, 10 L1i, 10 L2, 1 L3   | 10  | 23.0 | 184 | 7.7.747 | 127.791  
Esteban | amd20 | 2.6 | 32 KB, 32 KB, 512 KB, 16.38 MB | 64 L1d, 64 L1i, 64 L2, 1 L3   | 64  | 166.4 | 1331.2 | 16.469 | 60.113
John    | local | 2.6 | 384KB, 1.5MB, 12MB |6 L1, 6 L2, 1 L3 | 6 | 15.6 | 124.8| 13.895 | 71.248 
John    | intel18 | 2.4 | 32K, 32KB, 1MB, 28.160MB |20 L1d, 20 L1i, 20 L2, 1 L3 | 20 | 48.0 |384| 15.282 | 64.782          
Jorge   | local (Apple Silicon M2) | 3.2 | 192KB, 12MB, 8MB | 8 L1, 8 L2, 1 L3| 8 | 25.6 | X | 10.108 | 97.942
Jorge   | amd20-v100 | 2.4 | 32 KB, 32 KB, 1024 KB, 36.60MB | 64 L1d, 64 L1i, 64 L2, 1 L3 | 48 | 115.2 | 920 | 16.159 | 61.266
Farhad  | local | 2.6 | 64KB, 256KB, 12MB | 6 L1, 6 L2, 1 L3 | 6 | 15.6 | 249.6 | 11.389 | 86.826
Farhad  | intel16 | 2.4 | 32KB, 256KB, 35MB | 14 L1, 14 L2, 1 L3 | 14 | 33.6 | 336.11| 14.476 | 68.389

Finally, the Mflop/s of the matrix multiplication of size `N=100` is plotted against the Theoretical Peak Performance (Gflops/s) of the available CPUs for local and HPCC machine types. As this figure shows for the local machines, the rate of floating point operations (Mflops/s) is generally increased with an increase in the CPU computational capacity (TPP). However, for the HPCC machines, this trend is unexpectedly reversed; where the rate of the floating point operations decreases with an increase in TPP, especially for the more recent `amd20` machine which has 64 cores. `DO WE HAVE ANY OTHER REASON FOR THIS? MAYBE THE CALCULATION IS BANDWIDTH-BOUNDED? OR MAYBE HIGHER NUMBER OF CORES ARE MISLEADING AND NOT USED IN CALCULATIONS, BECAUSE WE RAN IN DEVELOPMENT NODES AND CORES ARE SHARED?.`

![img1](/analysis/Part1_Q4_Mflops_vs_TPP.png)


### Question 5:

The following figure shows the results for different matrix sizes from `N=1` to `N=1000`; where the horizontal axis shows the number of floating point operations ($N^3-N^2$) and vertical axis represents the average rate of the floating point operations. As this figure shows for both locan and HPCC runs, the rate of the floating point operation increases with an increase in the matrix size and reaches to a peak value. This peak value represents the ridge-point at which the bandwidth-bound and computing-bounds conflicted together. After this point, the matrix size is too large for the Cash replacements and therefore the rate of floating point operations reduces, due to the unavailability of the data in high-level memory cashes.

#### Discussion based on system architecture:

For instance in the local runs, the highest performance is for Esteban's systems which have the highest L1 cash capacity and common clock speed of 2.3 GHz. On the other hand, the lowest performance is for Farhad's system which has the lowest L1 cash capacity and clock speed of 2.4 GHz. Another interesting observation was that although John's system has slightly higher L1 cash capacity than that of Jorge, it's performance was a little lower. This can attribute to the considerably higher clock speed of the Jorge's system (3.2 GHz) which was about 20% higher.  

![img2](/analysis/Part1_Q5_Mflops-s_vs_Mflops.png)

### Question 6:

According to the figure, it was an interesting observation that the peak performance point of all graphs happens at `Mflops=3.91`, which corresponds to the matrix size of about `N=160`. `DO WE HAVE ANY OTHER REASON FOR THAT? CONSIDERING DIFFERENT ARCHITECTURES, IS THIS RESULT MAKE SENSE AT ALL?`

## Part 2: The Roofline Model

The CS Roofline Toolkit 1.1.0 has been cloned and run by each groupmember on the same computing systems used for Part 1. In what follows, the project questions are addressed:

### Question 1-3:

After cloning the Roofline Toolkit from GitHub, each groupmember modify the `config` file corresponds to the system used for running. The main modifications were disabling the OpenMM and MPI parts, changing the compiler to the gcc system compiler, removing the compiler options, and specify the output folders. 

Based on the results, the peak performance and bandwidths for different cash levels can be summarized in the following table:

User | Architecture | Peak Performance (Gflops/s) | L1 Cash (GB/s) | L2 Cash (GB/s) | DRAM (GB/s)
--|--|--|--|--|--|
Esteban | local      | 24.11 | 144.18 | 88.79 | 25.14 
Esteban | amd20      | 13.42 | 73.26  | N/A | 25.66 
John    | local      | X | X | X | X 
John    | intel18    | X | X | X | X 
Jorge   | local      | X | X | X | X 
Jorge   | amd20-v100 | X | X | X | X 
Farhad  | local      | 14.59 | 67.41  | 49.00 | 21.25 
Farhad  | intel16    | 11.96 | 86.47  | 68.15 | 16.77 
*N/A: Not Available

The resulting Roofline models for all computing systems of each groupmember can be found in the figure below. As each model in this figure shows, the horizontal cap of the model shows the maximum peak performance (comput-bound) where the inclined lines shows the limitations comes from the memory bandwidth capacity at different memory/cash level (bandwidth-bound).

![img3](/analysis/Part2_Q3_Rooflines.png)


