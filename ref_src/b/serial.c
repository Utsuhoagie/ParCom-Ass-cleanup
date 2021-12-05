#include <stdio.h>

#define N 100
int a[N][N], b[N][N], c[N][N];

int main()
{

    int i, j, k;

    for (i = 0; i < N; i++) //row of first matrix
    {
        for (j = 0; j < N; j++) //column of second matrix
        {

            int sum = 0;
            for (k = 0; k < N; k++)
            {
                a[i][k] = b[k][j] = 1;
                sum = sum + a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
    for (i = 0; i < N; i++)
    {

        for (j = 0; j < N; j++)
        {

            printf("%d  ", c[i][j]);
        }
        printf("\n");
    }
}
