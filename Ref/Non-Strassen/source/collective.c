#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000

int mt1[N][N], rs[N][N];

int main(int argc, char **argv)
{
    int i, j, k, l;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                mt1[i][j] = 1;
            }
        }
    }
    int token = N / size;
    int recv[token][N], tmp[token][N];

    MPI_Scatter(mt1, N * token, MPI_INT, recv, N * token, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(mt1, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < token; i++)
    {
        for (j = 0; j < N; j++)
        {
            int sum = 0;
            for (k = 0; k < N; k++)
            {
                sum += recv[i][k] * mt1[k][j];
            }
            tmp[i][j] = sum;
        }
    }

    MPI_Gather(tmp, N * token, MPI_INT, rs, N * token, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                printf("%d  ", rs[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}
