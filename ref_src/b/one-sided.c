#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

#define N 10000 // thay doi N lan luot la 100, 1000, 10000
#define MASTER 0

int b[N][N], c[N][N];
int a[N][N];

int main(int argc, char **argv) {
    int i, j, k, rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Win win;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            b[i][j] = 1;
        }
    }

    int walker = N / size;

    if (rank != MASTER)
        MPI_Win_create(a, walker * N, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
    else
        MPI_Win_create(c, N * N, sizeof(int), MPI_INFO_NULL, MPI_COMM_WORLD, &win);

    MPI_Win_fence(0, win);

    if (rank == MASTER) {
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                a[i][j] = 1;

        int s = 10;

        for (i = 1; i < size; i++)
            MPI_Put(a + (i)*walker, walker * N, MPI_INT, i, 0, walker * N, MPI_INT, win);


        MPI_Win_fence(1, win);
    }
    
    MPI_Win_fence(0, win);

    MPI_Win_fence(0, win);

    for (i = 0; i < walker; i++) {
        for (j = 0; j < N; j++) {
            int sum = 0;
            for (k = 0; k < N; k++)
                sum += a[i][k] * b[k][j];
                
            c[i][j] = sum;
        }
    }
    if (rank != MASTER) {
        MPI_Put(c, walker * N, MPI_INT, MASTER, (rank)*walker * N, walker * N, MPI_INT, win);
        MPI_Win_fence(1, win);
    }


    MPI_Win_fence(0, win);
    MPI_Win_free(&win);
    MPI_Finalize();
    return 0;
}
