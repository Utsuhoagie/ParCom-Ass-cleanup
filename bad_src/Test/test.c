#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>


int closestPow(int x) {
    int pow = 2;
    while (pow < x)
        pow *= 2;

    return pow;
}

void printArr(int* a, int size) {
    int i;

    for (i = 0; i < size; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void changeArr(int** M, int size) {
    int* N = malloc(size * sizeof(*N));
    int i;

    for (i = 0; i < size; i++) {
        if (i % 2 == 0) {
            N[i] = (*M)[i];
        }
        else {
            N[i] = -(*M)[i];
        }
    }

    *M = N;
}

int* changeArrReturn(int* M, int size) {
    int* N = malloc(size * sizeof(*N));
    int i;

    for (i = 0; i < size; i++) {
        if (i % 2 == 0) {
            N[i] = M[i];
        }
        else {
            N[i] = -M[i];
        }
    }

    return N;
}


void foo() {
    printf("aaaa\n");
}

void foo(int n) {
    printf("bbbb = %d\n", n);
}



int main() {
    //printf("size of double = %d", sizeof(double*));
    
    //int n = 10000;
    //printf("Closest pow of %d is %d", n, closestPow(n));



    // int size = 5;
    // int i;
    // int* M = malloc(size * sizeof(*M));
    
    // for (i = 0; i < size; i++)
    //     M[i] = i+1;

    // printArr(M, size);
    // M = changeArrReturn(M, size);
    
    // printf("Outside\n"); printArr(M, size);


    foo(5);


    // int* n = malloc(sizeof *n);
    // *n = 5;

    //free(n);

}