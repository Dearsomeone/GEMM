#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include "compute.h"

int main(int argc, char** argv)
{
	int N = 1000;
    unsigned int size;
    int i, j;
    int rank, numProcs;
    int localRows, localSize;
    int startRow;
    int endRow;
    float* A;
    float* B;
    float* C;
    float* localA; 
    float* localC;
    double* timeData;
    double start, finish, timeCost, maxTimeCost;
    char* endptr;
    FILE* file;
    
    /* 初始化 MPI */
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);       /* 获取当前进程的 rank */
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);   /* 获取总进程数 */

    /* 解析命令行参数（仅主进程解析）*/
    if (rank == 0) {
        for (i = 1; i < argc; i++) {
            switch (i) {
                case 1:
                    N = (int)strtol(argv[i], &endptr, 10);
                    printf("Command Line Argument %d: %d\n", i, N);
                    break;
                default:
                    printf("Warning: Ignore redundant command line arguments!");
                    break;
            }
        }
    }

    /* 广播 N 和 batchSize 到所有进程 */
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    size = N * N;

    /* 分配矩阵 B（所有进程）*/
    B = (float*)malloc(sizeof(float) * size);
    if (B == NULL) {
        printf("Memory allocation failed for B\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    /* 主进程分配 A, C，并初始化 */
    if (rank == 0) {
        A = (float*)malloc(sizeof(float) * size);
        C = (float*)malloc(sizeof(float) * size);
        if (A == NULL || C == NULL) {
            printf("Memory allocation failed for A or C\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        for (int i = 0; i < size; i++) {
            A[i] = sin(i);
            B[i] = cos(i);
        }
    }

    /* 广播矩阵 B 到所有进程 */
    MPI_Bcast(B, size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    localRows = N / numProcs;
    startRow = rank * localRows;
    endRow = (rank == numProcs - 1) ? N : startRow + localRows;
    localSize = (endRow - startRow) * N;

    /* 分配局部矩阵 A 和 C */
    localA = (float*)malloc(localSize * sizeof(float));
    localC = (float*)malloc(localSize * sizeof(float));
    if (localA == NULL || localC == NULL) {
        printf("Memory allocation failed for localA or localC\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    start = MPI_Wtime();
    /* 主进程分发矩阵 A 的行数据 */
    if (rank == 0) {
        for (i = 1; i < numProcs; i++) {
            int start = i * localRows;
            int end = (i == numProcs - 1) ? N : start + localRows;
            MPI_Send(&A[start * N], (end - start) * N, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }
        /* 主进程保留自己的部分 */
        memcpy(localA, A + startRow * N, localSize * sizeof(float));
    } else {
        MPI_Recv(localA, localSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    /* 每个进程计算自己的部分 */
    multiply(N, localA, B, localC, 0, endRow - startRow);

    /* 主进程收集结果 */
    if (rank == 0) {
        memcpy(C + startRow * N, localC, localSize * sizeof(float));
        for (int i = 1; i < numProcs; i++) {
            int start = i * localRows;
            int end = (i == numProcs - 1) ? N : start + localRows;
            MPI_Recv(&C[start * N], (end - start) * N, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        MPI_Send(localC, localSize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }
    
    finish = MPI_Wtime();
    timeCost = finish - start;
    /* 计算所有进程的最大时间 */
    MPI_Reduce(&timeCost, &maxTimeCost, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nTime to solution of the test: %.1f [sec]\n", timeCost);
        /* printMatrix(N, C); */
        /* 保存计时数据 */
        file = fopen("timing.txt", "a");
        if (file) {
            fprintf(file, "%.6f\n", maxTimeCost);
            fclose(file);
        } else {
            perror("Failed to open file");
        }
    }

    /* 释放内存 */
    free(localA);
    free(localC);
    free(B);
    if (rank == 0) {
        free(A);
        free(C);
    }
    
    /* 结束MPI */
    MPI_Finalize();

    return 0;
}