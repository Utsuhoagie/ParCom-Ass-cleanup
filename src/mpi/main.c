#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#include "math.h"
#include "string.h"

#define BREAKPOINT 8

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




typedef enum { TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT } QUADRANT;

int getQuad(int i, int j, int n) {
    if      (i < n/2 && j < n/2)    return TOP_LEFT;
    else if (i < n/2 && j >= n/2)   return TOP_RIGHT;
    else if (i >= n/2 && j < n/2)   return BOTTOM_LEFT;
    else if (i >= n/2 && j >= n/2)  return BOTTOM_RIGHT;
}



int getLowest(int n) {
    while (n % 2 == 0)
        n /= 2;

    return n;
}

int getNearest(int n) {
    int result = (n % 2 == 0)? n : n + 1;

    // if (result < BREAKPOINT)

    while (getLowest(result) >= BREAKPOINT)
        result += 2;

    return result;
}

void expandToNearest(int** pM, int n, int new_size) {
    int i, j, k;

    int* new_matrix = calloc(new_size * new_size, sizeof(*new_matrix));


    //memcpy(new_matrix, *pM, n * n * sizeof(int));
    for (i = 0; i < new_size; i++) {
        for (j = 0; j < new_size; j++) {
            if (i < n && j < n) {
                new_matrix[i*new_size + j] = (*pM)[i*n + j];
            }
            else {
                new_matrix[i*new_size + j] = 0;
            }
        }
    }

    // printf("Before expand, ");
    // print(*pM, n);

    // printf("After expand, ");
    // print(new_matrix, new_size);

    free(*pM);
    *pM = new_matrix;
}

void shrink(int** pM, int new_size, int n) {
    int i, j, k;

    int* og_matrix = calloc(n * n, sizeof(*og_matrix));

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            og_matrix[i*n + j] = (*pM)[i*new_size + j];
        }
    }

    // printf("After shrinking, ");
    // print(og_matrix, n);

    free(*pM);
    *pM = og_matrix;
}




/*
    n must be divisible by 2
*/
void split(int* M, int n,
           int** p1, int** p2, int** p3, int** p4) {
    int i, j, k;
    int quadrant;

    *p1 = init(n/2);
    *p2 = init(n/2);
    *p3 = init(n/2);
    *p4 = init(n/2);


    for (i = 0; i < n/2; i++)
        for (j = 0; j < n/2; j++)
            (*p1)[i*(n/2) + j] = M[i*n + j];

    k = 0;
    for (i = 0; i < n/2; i++)
        for (j = n/2; j < n; j++) {
            (*p2)[k] = M[i*n + j];
            k++;
        }

    k = 0;
    for (i = n/2; i < n; i++)
        for (j = 0; j < n/2; j++) {
            (*p3)[k] = M[i*n + j];
            k++;
        }

    k = 0;
    for (i = n/2; i < n; i++)
        for (j = n/2; j < n; j++) {
            (*p4)[k] = M[i*n + j];
            k++;
        }

}


/*

*/
void merge(int* M11, int* M12, int* M21, int* M22,
           int** pM, int n) {
    int i, j;
    int k11 = 0, k12 = 0, k21 = 0, k22 = 0;
    int quadrant;
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            quadrant = getQuad(i, j, n);

            switch(quadrant) {
              case TOP_LEFT:
                (*pM)[i*n + j] = M11[k11];
                k11++;
                break;
              case TOP_RIGHT:
                (*pM)[i*n + j] = M12[k12];
                k12++;
                break;
              case BOTTOM_LEFT:
                (*pM)[i*n + j] = M21[k21];
                k21++;
                break;
              case BOTTOM_RIGHT:
                (*pM)[i*n + j] = M22[k22];
                k22++;
                break;
            }
        }
    }
    

}


/*
    ===================================================
    == MUST INITIALIZE RESULT HERE, OR IN OPERATIONS!!!
    ===================================================
*/
int* add_recur(int* A, int* B, int n) {

    int* C; // NOTE: THIS MUST BE INITIALIZED ELSEWHERE!!!!
            //       LIKE FROM mul() OR init()


    if (n <= BREAKPOINT) {
        C = add(A, B, n);
        //print(C, n);
        return C;
    }
    else {
        int *A11, *A12, *A21, *A22;
        int *B11, *B12, *B21, *B22;
        int *C11, *C12, *C21, *C22;
        C = init(n);
        //*pC = init(n, ONE_RING);

        // NOTE: split will calloc A1...C22
        split(A, n, &A11, &A12, &A21, &A22);
        split(B, n, &B11, &B12, &B21, &B22);
        //split(C, n, &C1, &C2, &C21, &C22);

        C11 = add_recur(A11, B11, n/2);
        C12 = add_recur(A12, B12, n/2);
        C21 = add_recur(A21, B21, n/2);
        C22 = add_recur(A22, B22, n/2);

        // printf("A "); print(A, n);
        // printf("B "); print(B, n);

        // printf("A11 "); print(A11, n/2);
        // printf("A12 "); print(A12, n/2);
        // printf("A21 "); print(A21, n/2);
        // printf("A22 "); print(A22, n/2);
        // printf("B11 "); print(B11, n/2);
        // printf("B12 "); print(B12, n/2);
        // printf("B21 "); print(B21, n/2);
        // printf("B22 "); print(B22, n/2);

        // printf("C11 "); print(C11, n/2);
        // printf("C12 "); print(C12, n/2);
        // printf("C21 "); print(C21, n/2);
        // printf("C22 "); print(C22, n/2);
        printf("Merging! ");
        merge(C11, C12, C21, C22, &C, n);

        printf("C "); print(C, n);

        free(A11); free(A12); free(A21); free(A22);
        free(B11); free(B12); free(B21); free(B22);
        free(C11); free(C12); free(C21); free(C22);

        return C;
    }
}


int* mul_recur(int* A, int* B, int n) {

    int* C; // NOTE: THIS MUST BE INITIALIZED ELSEWHERE!!!!
            //       LIKE FROM mul() OR init()


    if (n <= BREAKPOINT) {
        printf("Mul between!\n");
        print(A, n);
        print(B, n);

        C = mul(A, B, n);
        printf("Result ");
        print(C, n);

        printf("\n");
        return C;
    }
    else {
        int *A11, *A12, *A21, *A22;
        int *B11, *B12, *B21, *B22;
        int *C11, *C12, *C21, *C22;
        C = init(n);

        // NOTE: split will calloc A1...C22
        split(A, n, &A11, &A12, &A21, &A22);
        split(B, n, &B11, &B12, &B21, &B22);

        int *A11_B11, *A12_B21, *A11_B12, *A12_B22,
            *A21_B11, *A22_B21, *A21_B12, *A22_B22;

        A11_B11 = mul_recur(A11, B11, n/2);
        A12_B21 = mul_recur(A12, B21, n/2);

        A11_B12 = mul_recur(A11, B12, n/2);
        A12_B22 = mul_recur(A12, B22, n/2);
        
        A21_B11 = mul_recur(A21, B11, n/2);
        A22_B21 = mul_recur(A22, B21, n/2);
        
        A21_B12 = mul_recur(A21, B12, n/2);
        A22_B22 = mul_recur(A22, B22, n/2);


        C11 = add(A11_B11, A12_B21, n/2);
        C12 = add(A11_B12, A12_B22, n/2);
        C21 = add(A21_B11, A22_B21, n/2);
        C22 = add(A21_B12, A22_B22, n/2);

        // printf("A "); print(A, n);
        // printf("B "); print(B, n);

        // printf("A11 "); print(A11, n/2);
        // printf("A12 "); print(A12, n/2);
        // printf("A21 "); print(A21, n/2);
        // printf("A22 "); print(A22, n/2);
        // printf("B11 "); print(B11, n/2);
        // printf("B12 "); print(B12, n/2);
        // printf("B21 "); print(B21, n/2);
        // printf("B22 "); print(B22, n/2);

        // printf("C11 "); print(C11, n/2);
        // printf("C12 "); print(C12, n/2);
        // printf("C21 "); print(C21, n/2);
        // printf("C22 "); print(C22, n/2);
        printf("Merging! ");
        merge(C11, C12, C21, C22, &C, n);

        printf("C "); print(C, n);

        free(A11_B11); free(A12_B21), free(A11_B12); free(A12_B22);
        free(A21_B11); free(A22_B21), free(A21_B12); free(A22_B22);

        free(A11); free(A12); free(A21); free(A22);
        free(B11); free(B12); free(B21); free(B22);
        free(C11); free(C12); free(C21); free(C22);

        return C;
    }
}

int* strassen(int* A, int* B, int n) {

    int* C; // NOTE: THIS MUST BE INITIALIZED ELSEWHERE!!!!
            //       LIKE FROM mul() OR init()


    if (n <= BREAKPOINT) {
        // printf("Mul between!\n");
        // print(A, n);
        // print(B, n);

        C = mul(A, B, n);
        // printf("Result ");
        // print(C, n);

        // printf("\n");
        return C;
    }

    else {
        // TODO: IMPLEMENT STRASSEN, NOT NORMAL MUL

        int *A11, *A12, *A21, *A22;
        int *B11, *B12, *B21, *B22;
        int *C11, *C12, *C21, *C22;
        C = init(n);


        // NOTE: split will calloc A1...C22
        split(A, n, &A11, &A12, &A21, &A22);
        split(B, n, &B11, &B12, &B21, &B22);


        // NOTE: 
        // ----- Strassen main --------------------
        // Initialize M1...7

        /*  M1 = (A11 + A22) * (B11 + B22)
            M2 = (A21 + A22) * B11
            M3 = A11 * (B12 - B22)
            M4 = A22 * (B21 - B11)
            M5 = (A11 + A12) * B22
            M6 = (A21 - A11) * (B11 + B12)
            M7 = (A12 - A22) * (B21 + B22)  */

        int *M1, *M2, *M3, *M4, *M5, *M6, *M7;

        int     *A11_add_A22 = add(A11, A22, n/2),
                *B11_add_B22 = add(B11, B22, n/2),
                *A21_add_A22 = add(A21, A22, n/2),
                *B12_sub_B22 = sub(B12, B22, n/2),
                *B21_sub_B11 = sub(B21, B11, n/2),
                *A11_add_A12 = add(A11, A12, n/2),
                *A21_sub_A11 = sub(A21, A11, n/2),
                *B11_add_B12 = add(B11, B12, n/2),
                *A12_sub_A22 = sub(A12, A22, n/2),
                *B21_add_B22 = add(B21, B22, n/2);

        M1 = strassen(A11_add_A22, B11_add_B22, n/2);
        M2 = strassen(A21_add_A22, B11        , n/2);
        M3 = strassen(A11        , B12_sub_B22, n/2);
        M4 = strassen(A22        , B21_sub_B11, n/2);
        M5 = strassen(A11_add_A12, B22        , n/2);
        M6 = strassen(A21_sub_A11, B11_add_B12, n/2);
        M7 = strassen(A12_sub_A22, B21_add_B22, n/2);

        int     *M1_add_M4 = add(M1, M4, n/2),
                *M5_sub_M7 = sub(M5, M7, n/2),
                *M1_sub_M2 = sub(M1, M2, n/2),
                *M3_add_M6 = add(M3, M6, n/2);

        /*  C11 = (M1 + M4) - (M5 - M7)
            C12 = M3 + M5
            C21 = M2 + M4
            C22 = (M1 - M2) + (M3 + M6) */

        C11 = sub(M1_add_M4, M5_sub_M7, n/2);
        C12 = add(M3, M5, n/2);
        C21 = add(M2, M4, n/2);
        C22 = add(M1_sub_M2, M3_add_M6, n/2);


        // --------------------------------------------
        // NOTE: printing debug
        // printf("A "); print(A, n);
        // printf("B "); print(B, n);

        // printf("A11 "); print(A11, n/2);
        // printf("A12 "); print(A12, n/2);
        // printf("A21 "); print(A21, n/2);
        // printf("A22 "); print(A22, n/2);
        // printf("B11 "); print(B11, n/2);
        // printf("B12 "); print(B12, n/2);
        // printf("B21 "); print(B21, n/2);
        // printf("B22 "); print(B22, n/2);

        // printf("C11 "); print(C11, n/2);
        // printf("C12 "); print(C12, n/2);
        // printf("C21 "); print(C21, n/2);
        // printf("C22 "); print(C22, n/2);


        // -------- Merge --------------
        //printf("Merging! ");
        merge(C11, C12, C21, C22, &C, n);

        //printf("C "); print(C, n);



        // -------- Cleanup ------------
        free(A11_add_A22); free(B11_add_B22);
        free(A21_add_A22); free(B12_sub_B22);
        free(B21_sub_B11); free(A11_add_A12);
        free(A21_sub_A11); free(B11_add_B12);
        free(A12_sub_A22); free(B21_add_B22);

        free(M1); free(M2); free(M3); free(M4); 
        free(M5); free(M6); free(M7); 

        free(M1_add_M4); free(M5_sub_M7);
        free(M1_sub_M2); free(M3_add_M6);

        free(A11); free(A12); free(A21); free(A22);
        free(B11); free(B12); free(B21); free(B22);
        free(C11); free(C12); free(C21); free(C22);

        return C;
    }
}


/*
void print_recur(int** pC, int n) {
    if (n < 4) {
        //C = add(A, B, n);
        printf("Base ");
        print(*pC, n);
        return;
    }

    else {
        int *C1, *C2, *C3, *C4;
        //C = add(A, B, n);
        split(*pC, n, &C1, &C2, &C3, &C4);

        print_recur(&C1, n/2);
        print_recur(&C2, n/2);
        print_recur(&C3, n/2);
        print_recur(&C4, n/2);

        //C = print_recur(C, n);

        free(C1); free(C2); free(C3); free(C4);

        return;
    }
} */

//#include "mpi.h"

int main(int argc, char** argv) {
    int n = 37;
    int* A = initStart(n, ONE);
    int* B = initStart(n, ONE);
    int* C;


    int new_size = getNearest(n);
    expandToNearest(&A, n, new_size);
    expandToNearest(&B, n, new_size);


    printf("\n\n ------ C ------ \n");
    C = strassen(A, B, new_size);

    // printf("\n\n ----- Before shrink, C ");
    // print(C, new_size);
    shrink(&C, new_size, n);
    printf("\n\n ----- Finished! In main, C ");
    print(C, n);


            // TESTING EXPAND/SHRINK
            // int new_size = expandToNearest(&A, n);
            // //printf("New size = %d\n", new_size);
            // printf("In main, ");
            // print(A, new_size);

            // shrink(&A, new_size, n);


    free(A); free(B); free(C);
}