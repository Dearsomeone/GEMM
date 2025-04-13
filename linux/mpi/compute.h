#ifndef COMPUTE_H
#define COMPUTE_H

/* N * N 方阵乘法 */
void multiply(int N, float* localA, float* B, float* localC, int localRows);
void printMatrix(int N, float* matrix);

#endif