#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>


// Choose how to initialize matrix in `initMat()`
typedef enum { ZERO, ONE_RING, ONE, CHECKER, INCR, DEC_RESET_INCR, RANDOM } Init;

// ----- Helpers ----------------

/*  Return closest power of 2 that's higher than x
    e.g: x = 5 -> return 8                          */
int closestHighPow(int x) {
    int pow = 2;
    while (pow < x)
        pow *= 2;

    return pow;
}

/*  Print matrix
    use this to debug!  */
void printMat(double* M, int n) {
    int i,j,k;

    printf("\nMatrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%2.0lf ", M[i*n + j]);

        printf("\n");
    }

    printf("\n");
}

// Print matrix as a 1D array
void printArr(double* M, int n) {
    int i;

    for (i = 0; i < n*n; i++)
        printf("%2.0lf ", M[i]);

    printf("\n\n");
}

/*  Initialize matrix with size = n, type = init
    then expand it to closest power of 2
    e.g: 5x5 -> 8x8                                 */
/*
double* initMat(double* M, int n, Init init) {
    double data;
    int i,j;

    switch(init) {
        case ZERO:
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    M[i*n + j] = 0;

            break;

        case ONE:
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    M[i*n + j] = 1;

            break;

        case ONE_RING:
            for (i = 0; i < n; i++) {
                M[i] = 1;
                M[(n-1)*n + i] = 1;
            }

            for (i = 0; i < n; i++) {
                M[i*n] = 1;
                M[i*n + (n-1)] = 1;
            }

            break;

        case CHECKER:
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    M[i*n + j] = (i + j) % 2;
            break;

        case INCR:
            data = 0;
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    M[i*n + j] = data;
                    data++;
                }
            }
            break;

        case DEC_RESET_INCR:
            for (i = 0; i < n; i++) {
                data = 0.0;
                for (j = 0; j < n; j++) {
                    data += 1.0 / (double)n;
                    M[i*n + j] = data;
                }
            }
            break;

        case RANDOM:

            break;

    }

    return M;
}
*/

void initMatStart(double** pM, int n, Init init) {
    // printf("Calloc  %d x %d\n", n, n);
    double *M = calloc(n * n, sizeof(*M));

    double data;
    int i,j;

    switch(init) {
        case ZERO:
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    M[i*n + j] = 0;

            break;

        case ONE:
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    M[i*n + j] = 1;

            break;

        case ONE_RING:
            for (i = 0; i < n; i++) {
                M[i] = 1;
                M[(n-1)*n + i] = 1;
            }

            for (i = 0; i < n; i++) {
                M[i*n] = 1;
                M[i*n + (n-1)] = 1;
            }

            break;

        case CHECKER:
            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    M[i*n + j] = (i + j) % 2;
            break;

        case INCR:
            data = 0;
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    M[i*n + j] = data;
                    data++;
                }
            }
            break;

        case DEC_RESET_INCR:
            for (i = 0; i < n; i++) {
                data = 0.0;
                for (j = 0; j < n; j++) {
                    data += 1.0 / (double)n;
                    M[i*n + j] = data;
                }
            }
            break;

        case RANDOM:

            break;

    }

    *pM = M;
}

void initMat(double** pM, int n) {
    // printf("  Calloc  %d x %d\n", n, n);
    double *M = calloc(n * n, sizeof(*M));

    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            M[i*n + j] = 0;

    *pM = M;
}

void reallocMat(double** pM, int n) {}



double* expand(double** pM, int n) {
    //int exp_n = closestHighPow(n);
    int exp_n = n + 1;    //(n % 2 == 1? n + 1: n);

    double* exp_M; initMat(&exp_M, exp_n);

    int i, j;

    for (i = 0; i < exp_n; i++) {
        for (j = 0; j < exp_n; j++) {
            if (i < n && j < n)
                exp_M[i*exp_n + j] = (*pM)[i*n + j];
            else
                exp_M[i*exp_n + j] = 0;
        }
    }

    // TODO: 
    // free(*pM);

    return exp_M;
}

/*  Trim matrix from expanded size = exp_n x exp_n
             down to original size = n x n
    e.g: 8x8 -> 5x5 or 6x6 or 7x7, depends on original size  */
double* trim(double* M, int n, int exp_n) {
    double* trimmed; initMat(&trimmed, n);
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            trimmed[i*n + j] = M[i*exp_n + j];

    free(M);

    return trimmed;
}

// ---- Matrix operations --------------
void add(double* A, double* B, double** pC, int n) {
    //printf("-- %s\n", __func__);
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            (*pC)[i*n + j] = A[i*n + j] + B[i*n + j];

}

void sub(double* A, double* B, double** pC, int n) {
    //printf("-- %s\n", __func__);
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            (*pC)[i*n + j] = A[i*n + j] - B[i*n + j];

}

/*  ONLY FOR SMALL MATRICES
    maybe 4x4 ?             */
void mul(double* A, double* B, double** pC, int n) {
    //printf("-- %s\n", __func__);
    int i,j,k;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < n; k++)
                (*pC)[i*n + j] += A[i*n + k] * B[k*n + j];

}

// ----- Strassen -------------------------------

/*  Split matrix (n x n, with n = power of 2)
    into 4 equal blocks
    and update pM11...pM22                      */
void split(double* M, double** pM11, double** pM12, double** pM21, double** pM22, int n) {
    int i, j, k;


    for (i = 0; i < n/2; i++)
        for (j = 0; j < n/2; j++)
            (*pM11)[i*(n/2) + j] = M[i*n + j];

    k = 0;
    for (i = 0; i < n/2; i++)
        for (j = n/2; j < n; j++) {
            (*pM12)[k] = M[i*n + j];
            k++;
        }

    k = 0;
    for (i = n/2; i < n; i++)
        for (j = 0; j < n/2; j++) {
            (*pM21)[k] = M[i*n + j];
            k++;
        }

    k = 0;
    for (i = n/2; i < n; i++)
        for (j = n/2; j < n; j++) {
            (*pM22)[k] = M[i*n + j];
            k++;
        }
}

/*  Merge 4 equal blocks (n/2 x n/2)
    into 1 result matrix (n x n)
    and return result matrix        */
void merge(double* C11, double* C12, double* C21, double* C22, int n, double** pC) {
    int big_n = n*2;
    int i, j;


    printMat(C11, n);
    printMat(C12, n);
    printMat(C21, n);
    printMat(C22, n);
    printMat(*pC, big_n);

    //printf("\tStarting C combine!!\n");
    for (i = 0; i < big_n; i++) {
        for (j = 0; j < big_n; j++) {
            bool isLeft  = j < (big_n / 2),
                 isUpper = i < (big_n / 2);

            if (isUpper) {
                if (isLeft)
                    (*pC)[i*big_n + j] = C11[i*n + j];
                else
                    (*pC)[i*big_n + j] = C12[i*n + j%n];
            }
            else {
                if (isLeft)
                    (*pC)[i*big_n + j] = C21[(i%n)*n + j];
                else
                    (*pC)[i*big_n + j] = C22[(i%n)*n + j%n];
            }
        }
    }

    printMat(*pC, big_n);
}


/*
    1/ Call strassen() recursively to split into 4 blocks

    2/ Calculate M1...M7
    if current matrix size (before split) > 4x4:
        call strassen() from M1...M7
    else:
        use simple O(n^3) multiplication from M1...M7

    3/ Calculate C11...C22 using M1...M7

    4/ Merge C11...C22 => return C
*/
int strassen(double* A, double* B, double** pC, int n, double* exp_n) {
	//if (n % 16 == 0)
	//	printf("- ");
    //double* C; // = malloc(n * n * sizeof(double));


    // Small enough to use normal multiplication
    if (n <= 2) {
        //C = mul(A, B, n);
        printf("  BASE MUL \n");
        mul(A, B, pC, n);
        return n;
    }


    printf("\n\n=== Strassen with %d x %d, n = %d, *exp_n = %lf\n", n, n, n, *exp_n);
    // If not even, expand to even
    // 5x5 -> 6x6
    if (n % 2 != 0) {
        
        A = expand(&A, n);
        B = expand(&B, n);
        *pC = expand(pC, n);
        n++;
        *exp_n += 0.25; // because 4 splits will call this


        //n = closestHighPow(n);
    }
    
    printf("A "); printMat(A, n);
    printf("B "); printMat(B, n);
    printf("C "); printMat(*pC, n);

    // Matrices after splitting into 4
    double *A11, *A12, *A21, *A22;
    double *B11, *B12, *B21, *B22;
    double *C11, *C12, *C21, *C22;

    initMat(&A11, n / 2); initMat(&A12, n / 2);
    initMat(&A21, n / 2); initMat(&A22, n / 2);
    initMat(&B11, n / 2); initMat(&B12, n / 2);
    initMat(&B21, n / 2); initMat(&B22, n / 2);
    initMat(&C11, n / 2); initMat(&C12, n / 2);
    initMat(&C21, n / 2); initMat(&C22, n / 2);

    printf("--- Finish calloc for A/B/C 11...22!\n\n");

    // Indices
    int i,j,k;

    // printf("--- A matrix ---\n");
    // printMat(A, n);
    // printf("--- B matrix ---\n");
    // printMat(B, n);

    split(A, &A11, &A12, &A21, &A22, n);
    split(B, &B11, &B12, &B21, &B22, n);


    // printf(" -- Finished partition! Starting M calc ------------\n\n");

    // M matrices for calculating
    double *M1, *M2, *M3, *M4, *M5, *M6, *M7;
    initMat(&M1, n/2);
    initMat(&M2, n/2);
    initMat(&M3, n/2);
    initMat(&M4, n/2);
    initMat(&M5, n/2);
    initMat(&M6, n/2);
    initMat(&M7, n/2);

    printf("\n--- Finished calloc for M1...7!\n\n");



    /*  M1 = (A11 + A22) * (B11 + B22)
        M2 = (A21 + A22) * B11
        M3 = A11 * (B12 - B22)
        M4 = A22 * (B21 - B11)
        M5 = (A11 + A12) * B22
        M6 = (A21 - A11) * (B11 + B12)
        M7 = (A12 - A22) * (B21 + B22)  */

    double  *A11_add_A22,
            *B11_add_B22,
            *A21_add_A22,
            *B12_sub_B22,
            *B21_sub_B11,
            *A11_add_A12,
            *A21_sub_A11,
            *B11_add_B12,
            *A12_sub_A22,
            *B21_add_B22;
    
    initMat(&A11_add_A22, n/2);
    initMat(&B11_add_B22, n/2);
    initMat(&A21_add_A22, n/2);
    initMat(&B12_sub_B22, n/2);
    initMat(&B21_sub_B11, n/2);
    initMat(&A11_add_A12, n/2);
    initMat(&A21_sub_A11, n/2);
    initMat(&B11_add_B12, n/2);
    initMat(&A12_sub_A22, n/2);
    initMat(&B21_add_B22, n/2);

    add(A11, A22, &A11_add_A22, n/2),
    add(B11, B22, &B11_add_B22, n/2),
    add(A21, A22, &A21_add_A22, n/2),
    sub(B12, B22, &B12_sub_B22, n/2),
    sub(B21, B11, &B21_sub_B11, n/2),
    add(A11, A12, &A11_add_A12, n/2),
    sub(A21, A11, &A21_sub_A11, n/2),
    add(B11, B12, &B11_add_B12, n/2),
    sub(A12, A22, &A12_sub_A22, n/2),
    add(B21, B22, &B21_add_B22, n/2);

    // M1 = strassen(add(A11, A22, n/2), add(B11, B22, n/2), n/2);
    // M2 = strassen(add(A21, A22, n/2), B11               , n/2);
    // M3 = strassen(A11               , sub(B12, B22, n/2), n/2);
    // M4 = strassen(A22               , sub(B21, B11, n/2), n/2);
    // M5 = strassen(add(A11, A12, n/2), B22               , n/2);
    // M6 = strassen(sub(A21, A11, n/2), add(B11, B12, n/2), n/2);
    // M7 = strassen(sub(A12, A22, n/2), add(B21, B22, n/2), n/2);

    strassen(A11_add_A22, B11_add_B22, &M1, n/2, exp_n);
    strassen(A21_add_A22, B11        , &M2, n/2, exp_n);
    strassen(A11        , B12_sub_B22, &M3, n/2, exp_n);
    strassen(A22        , B21_sub_B11, &M4, n/2, exp_n);
    strassen(A11_add_A12, B22        , &M5, n/2, exp_n);
    strassen(A21_sub_A11, B11_add_B12, &M6, n/2, exp_n);
    strassen(A12_sub_A22, B21_add_B22, &M7, n/2, exp_n);

    printf("\n--- Finished calc for M1...7!\n\n");



    /*  C11 = (M1 + M4) - (M5 - M7)
        C12 = M3 + M5
        C21 = M2 + M4
        C22 = (M1 - M2) + (M3 + M6) */

    double  *M1_add_M4,
            *M5_sub_M7,
            *M1_sub_M2,
            *M3_add_M6;

    initMat(&M1_add_M4, n/2);
    initMat(&M5_sub_M7, n/2);
    initMat(&M1_sub_M2, n/2);
    initMat(&M3_add_M6, n/2);
    
    add(M1, M4, &M1_add_M4 ,n/2);
    sub(M5, M7, &M5_sub_M7 ,n/2);
    sub(M1, M2, &M1_sub_M2 ,n/2);
    add(M3, M6, &M3_add_M6 ,n/2);

    sub(M1_add_M4, M5_sub_M7, &C11, n/2);
    add(M3       , M5       , &C12, n/2);
    add(M2       , M4       , &C21, n/2);
    add(M1_sub_M2, M3_add_M6, &C22, n/2);

    printf("\n--- Finished calc for C11...22!");


    printf("\n--- Combining to form C matrix --------------\n\n");

    merge(C11, C12, C21, C22, n/2, pC);

    printf("\n--- Finished combining C! --------------\n\n");

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

    //free(A); free(B); //free(*pC);

    return n;
}

int strassenMain(double* A, double* B, double** pC, int n) {
    double exp_n = (double) n; //(n % 2 == 1? n + 1 : n);

    strassen(A, B, pC, n, &exp_n);

    // free(C);    // must free before trimming

    //printArr(C, exp_n);



    // double** C_out;

    // trim(C, C_out, n, exp_n);

    // printMat(*C_out, n);

    // return C_out;

    //printf("=== Strassen main, before trim, exp_n = %lf\n", exp_n);
    //*pC = trim(*pC, n, (int) exp_n);

    return n;
}

/*  =======================================================================
    ======================================================================= */


int main(int argc, char** argv) {

// ------------------------------------------
    // Declarations
    //int n = argv[1]? atoi(argv[1]) : 3, 
    int     n = 5,
        exp_n = n;

    // Matrices
    double* A; initMatStart(&A, n, ONE_RING); //calloc(n * n, sizeof(double));
    double* B; initMatStart(&B, n, ONE_RING); //calloc(n * n, sizeof(double));
    double* C; initMatStart(&C, n, ZERO);     // = malloc(n * n * sizeof(double));
    
    //A = initMat(A, n, ONE_RING);
    //B = initMat(B, n, ONE_RING);
    //C = initMat(C, n, ZERO);

    exp_n = (n % 2 == 1? n + 1 : n);


// ---------------------------------------
clock_t startTime, endTime, totalTime;
startTime = clock();

        // --- Print only ----
        // printMat(A, exp_n);
        // printMat(B, exp_n);
        // printMat(C, exp_n);

        // --- Other ops -----
        // C = add(A, B, n);
        // printMat(C, n);


        // --- Normal mul ----
        // C = mul(A, B, n);
        // C = trim(&C, n, exp_n);
        // printMat(C, n);


        // --- Strassen mul ---
        n = strassenMain(A, B, &C, n);          /* TODO: This has 39 callocs  */
        //C = trim(&C, n, exp_n);

        printf("\n\n=====================================");
        printf("\n========== After Strassen: ==========\n");
        printMat(C, n % 2 == 0? n : n+1);



startTime = clock() - startTime;
totalTime = endTime - startTime;
printf("\nTotal time = %lf\n", (double)startTime / CLOCKS_PER_SEC);
// ----------------------------------------

    free(A); 
    free(B); 
    free(C);
}
