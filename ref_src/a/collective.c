#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MATSIZE 100

int matrixA[MATSIZE][MATSIZE], matrixB[MATSIZE][MATSIZE], matrixC[MATSIZE][MATSIZE];
int receive[MATSIZE][MATSIZE], tempResult[MATSIZE][MATSIZE];

int main(int argc, char **argv)
{
    int i, j, k; // count variable
    int rank, size;
    double start, finish; // for running time

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    start = MPI_Wtime();

    // Create matrix A and B
    if (rank == 0)
    {
        for (i = 0; i < MATSIZE; i++)
        {
            for (j = 0; j < MATSIZE; j++)
            {
                matrixA[i][j] = 1;
                matrixB[i][j] = 1;
            }
        }
    }

    int averageRow = MATSIZE / size;

    // cast
    MPI_Scatter(matrixA, averageRow * MATSIZE, MPI_INT, receive, averageRow * MATSIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrixB, MATSIZE * MATSIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Matrix multi of each process
    for (i = 0; i < averageRow; i++)
    {
        for (j = 0; j < MATSIZE; j++)
        {
            int sum = 0;
            for (k = 0; k < MATSIZE; k++)
            {
                sum += receive[i][k] * matrixB[k][j];
            }
            tempResult[i][j] = sum;
        }
    }

    // process 0 gather temp of each process
    MPI_Gather(tempResult, averageRow * MATSIZE, MPI_INT, matrixC, averageRow * MATSIZE, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        finish = MPI_Wtime();
        printf("Matrix multiplication finish!\n");
    //     for (i = 0; i < MATSIZE; i++)
    //     {
    //         for (j = 0; j < MATSIZE; j++)
    //         {
    //             printf("%d  ", matrixC[i][j]);
    //         }
    //         printf("\n");
    //     }
        printf("Computing time: %f\n", finish - start);
    }

    MPI_Finalize();
    return 0;
}