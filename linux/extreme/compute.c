#include <math.h>
#include <stdio.h>
#include "compute.h"
#include <omp.h>

void multiply(int N, float* restrict A, float* restrict B, float* restrict C, int startRow, int endRow)
{
    int i, j, k;
    #pragma omp parallel for
	for (i = startRow; i < endRow; i++) 
    {
        for (k = 0; k < N; k++) 
        {
            for (j = 0; j < N; j++) 
            {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

void printMatrix(int N, float* matrix)
{
	int i, j;
	for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++) 
        {
            printf("%8.2f ", matrix[i * N + j]);
        }
        printf("\n");
    }
}