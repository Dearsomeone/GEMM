#include "compute.h"
#include <math.h>
#include <stdio.h>

void multiply(int N, float* localA, float* B, float* localC, int localRows)
{
    int i, j, k;
    for (i = 0; i < localRows; i++)
    {
        for (k = 0; k < N; k++)
        {
            for (j = 0; j < N; j++)
            {
                localC[i * N + j] += localA[i * N + k] * B[k * N + j];
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
