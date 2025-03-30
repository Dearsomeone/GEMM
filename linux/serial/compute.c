#include <math.h>
#include <stdio.h>
#include "compute.h"

/* ijk遍历，缓存命中率低 */
void multiply(int N, float* A, float* B, float* C)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

void printMatrix(int N, float* matrix)
{	
    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            printf("%8.2f ", matrix[i * N + j]);
        }
        printf("\n");
    }
}
