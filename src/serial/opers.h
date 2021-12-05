#include "init.h"


int* add(int* A, int* B, int n) {
    int* C = init(n);
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C[i*n + j] = A[i*n + j] + B[i*n + j];

    return C;
}


int* sub(int* A, int* B, int n) {
    int* C = init(n);
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C[i*n + j] = A[i*n + j] - B[i*n + j];

    return C;
}


/*  ONLY FOR SMALL MATRICES
    maybe 4x4 ?             */
int* mul(int* A, int* B, int n) {
    int* C = init(n);
    int i,j,k;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < n; k++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];

    return C;
}
