#include <math.h>
#include <stdio.h>
#include "compute.h"

void multiply(int N, float* A, float* B, float* C)
{
	int i, j, k;

	for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
			C[i * N + j] = 0;
            for (k = 0; k < N; k++) {
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