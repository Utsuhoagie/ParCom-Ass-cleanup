#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MATSIZE 100
#define MASTER 0      /* taskid of first task */
#define FROM_MASTER 1 /* setting a message type */
#define FROM_WORKER 2 /* setting a message type */

double a[MATSIZE][MATSIZE], /* matrix A to be multiplied */
    b[MATSIZE][MATSIZE],    /* matrix B to be multiplied */
    c[MATSIZE][MATSIZE];    /* result matrix C */
int main(int argc, char *argv[])
{
    int numtasks,              /* number of tasks in partition */
        taskId,                /* a task identifier */
        numworkers,            /* number of worker tasks */
        source,                /* task id of message source */
        dest,                  /* task id of message destination */
        mtype,                 /* message type */
        rows,                  /* rows of matrix A sent to each worker */
        averow, extra, offset, /* used to determine rows sent to each worker */
        i, j, k;               /* misc */
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskId);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    if (numtasks < 2)
    {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Finalize();
        exit(1);
    }

    numworkers = numtasks - 1;

    /**************************** master task ************************************/
    if (taskId == MASTER)
    {
        printf("mpi_mm has started with %d tasks.\n", numtasks);
        // printf("Initializing arrays...\n");
        for (i = 0; i < MATSIZE; i++)
            for (j = 0; j < MATSIZE; j++)
                a[i][j] = 1;
        for (i = 0; i < MATSIZE; i++)
            for (j = 0; j < MATSIZE; j++)
                b[i][j] = 1;

        /* Measure start time */
        double start = MPI_Wtime();

        /* Send matrix data to the worker tasks */
        averow = MATSIZE / numworkers;
        extra = MATSIZE % numworkers;
        offset = 0;
        mtype = FROM_MASTER;
        for (dest = 1; dest <= numworkers; dest++)
        {
            rows = (dest <= extra) ? averow + 1 : averow;
            // printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows * MATSIZE, MPI_DOUBLE, dest, mtype,
                     MPI_COMM_WORLD);
            MPI_Send(&b, MATSIZE * MATSIZE, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset = offset + rows;
        }

        /* Receive results from worker tasks */
        mtype = FROM_WORKER;
        for (i = 1; i <= numworkers; i++)
        {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows * MATSIZE, MPI_DOUBLE, source, mtype,
                     MPI_COMM_WORLD, &status);
            // printf("Received results from task %d\n",source);
        }

        /* Print results */

        /* Measure finish time */
        double finish = MPI_Wtime();
        printf("Matrix multiplication finish!\n");
        // for (i = 0; i < MATSIZE; i++)
        // {
        //     for (j = 0; j < MATSIZE; j++)
        //     {
        //         printf("%f  ", c[i][j]);
        //     }
        //     printf("\n");
        // }
        printf("Computing time: %f\n", finish - start);
    }

    /**************************** worker task ************************************/
    if (taskId > MASTER)
    {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows * MATSIZE, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, MATSIZE * MATSIZE, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        for (k = 0; k < MATSIZE; k++)
            for (i = 0; i < rows; i++)
            {
                c[i][k] = 0.0;
                for (j = 0; j < MATSIZE; j++)
                    c[i][k] = c[i][k] + a[i][j] * b[j][k];
            }
        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&c, rows * MATSIZE, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}