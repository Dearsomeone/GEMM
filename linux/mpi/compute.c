#include "compute.h"
#include <math.h>
#include <stdio.h>

void multiply(int N, float* A, float* B, float* C, int startRow, int endRow)
{
    for (int i = startRow; i < endRow; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

void printMatrix(int N, float* matrix)
{
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%8.2f ", matrix[i * N + j]);
        }
        printf("\n");
    }
}
