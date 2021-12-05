#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#include "math.h"
#include "string.h"

typedef enum { ZERO, ONE, ONE_RING } Init;

int* initStart(int n, Init init) {
    int* new_matrix = malloc(n * n * sizeof(*new_matrix));
    int i, j;

    switch(init) {
      case ZERO:
        memset(new_matrix, 0, n * n * sizeof(*new_matrix));
        break;
      case ONE:
        for (i = 0; i < n*n; i++)
            new_matrix[i] = 1;
        break;
      case ONE_RING:
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                new_matrix[i*n + j] = ((i * j == 0) || (i == n-1) || (j == n-1)) ?
                                        1 : 0;
        break;
    }

    return new_matrix;
}

int* init(int n) {
    int* new_matrix = calloc(n * n, sizeof(*new_matrix));

    return new_matrix;
}

void print(int* M, int n) {
    int i, j;

    printf("Matrix:");

    for (i = 0; i < n; i++) {
        printf("\n\t");
        for (j = 0; j < n; j++) {
            char* space = M[i*n + j] < 10? " " : "";
            printf("%s%d ", space, M[i*n  + j]);
        }
    }
    printf("\n");
}