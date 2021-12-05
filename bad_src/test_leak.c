#include "stdio.h"
#include "stdlib.h"

typedef enum { ZERO, ONE, ONE_RING } Init;

void print(int* A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", A[i*n + j]);
        }
        printf("\n");
    }
}

void init(int** pA, int n, Init init) {
    *pA = calloc(n * n, sizeof(**pA));

    switch(init) {
        case ZERO:
            for (int i = 0; i < n*n; i++)
                (*pA)[i] = 0;
            break;

        case ONE:
            for (int i = 0; i < n*n; i++)
                (*pA)[i] = 1;
            break;

        case ONE_RING:
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (i == 0 || i == n-1 || j == 0 || j == n-1)
                        (*pA)[i*n + j] = 1;
                    else
                        (*pA)[i*n + j] = 0;
                }
            }
            break;
    }
}




/*
    =============================
    ===== MATRIX OPERATIONS =====
*/

void add(int* A, int* B, int** pC,  int n) {
    // int* C = malloc(sizeof(*C) * n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            (*pC)[i*n + j] = A[i*n + j] + B[i*n + j];
        }
    }
}




void recurAdd(int* A, int* B, int** pC, int n, int times) {
    // returns A + B*times
    if (times == 1) {
        add(A, B, pC, n);
        return;
    }

    int* AB;
    init(&AB, n, ZERO);

    recurAdd(A, B, &AB, n, times-1);
    add(A, AB, pC, n);
    free(AB);
}


int main(int argc, char **argv) {
    int n = 5;
    int *A;
    int *B;
    int *C;

    init(&A, n, ONE);
    init(&B, n, ONE);
    init(&C, n, ZERO);

    //add(A, B, &C, n);
    recurAdd(A, B, &C, n, 5);

    print(C, n);

    free(A); free(B); free(C);
}