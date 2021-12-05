#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

#define MATSIZE 100
#define MASTER 0

int matrixA[MATSIZE][MATSIZE], matrixB[MATSIZE][MATSIZE]; // Matrix A and B for multiplication
int matrixC[MATSIZE][MATSIZE];                            // Matrix C for storing result

int main(int argc, char **argv)
{
    int i, j, k, rank, size;
    double start, finish; // for running time

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Win win;

    start = MPI_Wtime();

    // Init matrix B for all process
    for (i = 0; i < MATSIZE; i++)
    {
        for (j = 0; j < MATSIZE; j++)
        {
            matrixB[i][j] = 1;
        }
    }

    int walker = MATSIZE / size;

    // Create Windows for Worker so Master can put data of A
    if (rank != MASTER)
    {
        MPI_Win_create(matrixA, walker * MATSIZE, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    }
    // Create window for Master
    else
    {
        MPI_Win_create(matrixC, MATSIZE * MATSIZE, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    }

    // put data of matrix A to workers
    MPI_Win_fence(0, win);

    if (rank == MASTER)
    {

        for (i = 0; i < MATSIZE; i++)
        {

            for (j = 0; j < MATSIZE; j++)
            {
                matrixA[i][j] = 1;
            }
        }

        for (i = 1; i < size; i++)
        {

            MPI_Put(matrixA + (i)*walker, walker * MATSIZE, MPI_INT, i, 0, walker * MATSIZE, MPI_INT, win);
        }
        MPI_Win_fence(1, win);
    }
    MPI_Win_fence(0, win);

    MPI_Win_fence(0, win);

    for (i = 0; i < walker; i++)
    {
        for (j = 0; j < MATSIZE; j++)
        {
            int sum = 0;
            for (k = 0; k < MATSIZE; k++)
            {
                sum += matrixA[i][k] * matrixB[k][j];
            }
            matrixC[i][j] = sum;
        }
    }
    if (rank != MASTER)
    {
        MPI_Put(matrixC, walker * MATSIZE, MPI_INT, MASTER, (rank)*walker * MATSIZE, walker * MATSIZE, MPI_INT, win);
        MPI_Win_fence(1, win);
    }

    if (rank == 0)
    {
        finish = MPI_Wtime();
        printf("Matrix multiplication finish!\n");
        // for (i = 0; i < MATSIZE; i++)
        // {
        //     for (j = 0; j < MATSIZE; j++)
        //     {
        //         printf("%d  ", matrixC[i][j]);
        //     }
        //     printf("\n");
        // }
        printf("Computing time: %f\n", finish - start);
    }

    MPI_Win_fence(0, win);
    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}
