#include <stdio.h>
#include <time.h>

#define MATSIZE 100
int matrixA[MATSIZE][MATSIZE], matrixB[MATSIZE][MATSIZE], matrixC[MATSIZE][MATSIZE];

int main()
{
    int i, j, k;
    clock_t begin = clock();

    for (i = 0; i < MATSIZE; i++) //row of first matrix
    {
        for (j = 0; j < MATSIZE; j++) //column of second matrix
        {

            int sum = 0;
            for (k = 0; k < MATSIZE; k++)
            {
                matrixA[i][k] = matrixB[k][j] = 1;
                sum = sum + matrixA[i][k] * matrixB[k][j];
            }
            matrixC[i][j] = sum;
        }
    }

    clock_t end = clock();
    printf("Matrix multiplication finish!\n");
    // for (i = 0; i < MATSIZE; i++)
    // {
    //     for (j = 0; j < MATSIZE; j++)
    //     {
    //         printf("%d  ", matrixC[i][j]);
    //     }
    //     printf("\n");
    // }
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Computing time: %f\n", time_spent);

    return 0;
}
