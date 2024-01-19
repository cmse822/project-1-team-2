# Report

Team Members: Esteban Echeverri, John Salako, Farhad Abdollahi, Jorge Martinez-Ortiz.

## Warm-up Answers

Kernel | Arithmetic Intensity | FLOPs/byte
--|--|--
Y[j] += Y[j] + A[j][i] * B[i] | 3/4   | 3/32 
s += A[i] * A[i]              | 2     | 1/4
s += A[i] * B[i]              | 1     | 1/8
Y[i] = A[i] + C*B[i]          | 2/3   | 1/12

## Part 1: Matrix-matrix Multiplication

### Operator Analysis
In the matrix multiplication $AB = C$, we have that a single column of $C$ is given by

$$c_i = \sum_{k=1}^N a_{ik}b_{ki}.$$

The following operations are required:
- Vector product: $N$ multiplications and $N-1$ additions.
- Each column of $C$ has $N$ entries.
- $C$ has $N$ columns.

The total number of operations requires is $N^3-N^2=O(N^3)$.

### Architectures 

 Architecture | Time (s) $N=1000$|Mflops/s $N=1000$ | Clock speed (GHz) | Cache Size (Kb)| Cache Layout | Cores | Theoretical Peak Performance (GFlops/s)|
--|--|--|--|--|--|--|--|
Esteban local           | |||
amd20                   | |||
John local              | |||
intel18                 | |||
Farhad local            |X |X | 2.6  | 64, 256, 12MB | 6, 6, 1 | 6  | 249.6
intel16                 |X |X | 3.001| 32, 256, 35MB | 14, 14, 1 | 14 | 336.11
Jorge Apple Silicon M2  |0.0323759 |||
amd20-v100              | |||

![img1](/analysis/sample.png)

## Part 2: The Roofline Model