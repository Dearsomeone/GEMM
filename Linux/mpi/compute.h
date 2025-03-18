#ifndef COMPUTE_H
#define COMPUTE_H

/* N * N 方阵乘法 */
void multiply(int N, float* A, float* B, float* C, int startRow, int endRow);
void printMatrix(int N, float* matrix);

#endif