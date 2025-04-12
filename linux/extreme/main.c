#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>
#include <omp.h> 
#include "compute.h"

int main(int argc, char** argv)
{
    int N = 1000;
    unsigned int size;
    int i;
    int rank, numProcs;
    int localRows, localSize;
    int numThreads = 28;
    float* A = NULL;
    float* B = NULL;
    float* C = NULL;
    float* localA = NULL;
    float* localC = NULL;
    double start, finish, timeCost, maxTimeCost;
    char* endptr;
    FILE* file;

    /* 初始化 MPI */
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);       /* 获取当前进程的 rank */
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);   /* 获取总进程数 */
    printf("Hello World from process %d of %d\n", rank, numProcs);

    /* 解析命令行参数（仅主进程解析）*/
    if (rank == 0) {
        for (i = 1; i < argc; i++) {
            switch (i) {
                case 1:
                    N = (int)strtol(argv[i], &endptr, 10);
                    printf("Command Line Argument %d: %d\n", i, N);
                    break;
                case 2:
                    numThreads = (int)strtol(argv[i], &endptr, 10);
                    printf("Command Line Argument %d: %d\n", i, numThreads);
                    break;
                default:
                    printf("Warning: Ignore redundant command line arguments!");
                    break;
            }
        }
    }
    omp_set_num_threads(numThreads);
    /* 广播 N 到所有进程 */
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
        /* 初始化数组 */
        memset(C, 0, size * sizeof(float));
        for (int i = 0; i < size; i++) {
            A[i] = sin(i);
            B[i] = cos(i);
        }
    }

    /* 广播矩阵 B 到所有进程 */
    MPI_Bcast(B, size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* 计算每个进程的行数和偏移量 */
    int* sendcounts = (int*)malloc(numProcs * sizeof(int));
    int* displs = (int*)malloc(numProcs * sizeof(int));

    int rowsPerProcess = N / numProcs;
    int remainder = N % numProcs;

    for (int i = 0; i < numProcs; i++) {
        sendcounts[i] = (i < remainder) ? (rowsPerProcess + 1) * N : rowsPerProcess * N;
        displs[i] = (i == 0) ? 0 : displs[i - 1] + sendcounts[i - 1];
    }

    localRows = sendcounts[rank] / N;
    localSize = sendcounts[rank];

    /* 分配局部矩阵 A 和 C */
    localA = (float*)malloc(localSize * sizeof(float));
    localC = (float*)malloc(localSize * sizeof(float));
    if (localA == NULL || localC == NULL) {
        printf("Memory allocation failed for localA or localC\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    memset(localC, 0, localSize * sizeof(float));  // 初始化 localC

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    /* 主进程分发矩阵 A 的行数据 */
    MPI_Scatterv(A, sendcounts, displs, MPI_FLOAT, localA, localSize, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* 每个进程计算自己的部分 */
    multiply(N, localA, B, localC, 0, localRows);

    /* 主进程收集结果 */
    MPI_Gatherv(localC, localSize, MPI_FLOAT, C, sendcounts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime();
    timeCost = finish - start;

    /* 计算所有进程的最大时间 */
    MPI_Reduce(&timeCost, &maxTimeCost, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nTime to solution of the test: %.1f [sec]\n", timeCost);
        // printMatrix(N, C);
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
    free(sendcounts);
    free(displs);
    if (rank == 0) {
        free(A);
        free(C);
    }

    /* 结束MPI */
    MPI_Finalize();

    return 0;
}