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

double* initMat(int n, Init init) {
    printf("Calloc once! %d x %d\n", n, n);
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

    return M;
}




double* expand(double* M, int n) {
    //int expanded_n = closestHighPow(n);
    int expanded_n = n + 1;    //(n % 2 == 1? n + 1: n);

    double* expanded_M = initMat(expanded_n, sizeof(*expanded_M));

    int i, j;

    for (i = 0; i < expanded_n; i++) {
        for (j = 0; j < expanded_n; j++) {
            if (i < n && j < n)
                expanded_M[i*expanded_n + j] = M[i*n + j];
            else
                expanded_M[i*expanded_n + j] = 0;
        }
    }

    return expanded_M;
}

/*  Trim matrix from expanded size = expanded_n x expanded_n
             down to original size = n x n
    e.g: 8x8 -> 5x5 or 6x6 or 7x7, depends on original size  */
double* trim(double* M, int n0, int n) {
    double* trimmed = initMat(n0, ZERO);
    int i,j;

    for (i = 0; i < n0; i++)
        for (j = 0; j < n0; j++)
            trimmed[i*n0 + j] = M[i*n + j];

    return trimmed;
}

// ---- Matrix operations --------------
double* add(double* A, double* B, int n) {
    double* C = initMat(n, ZERO);
    printf("-- %s\n", __func__);
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C[i*n + j] = A[i*n + j] + B[i*n + j];

    return C;
}

double* sub(double* A, double* B, int n) {
    double* C = initMat(n, ZERO);
    printf("-- %s\n", __func__);
    int i,j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C[i*n + j] = A[i*n + j] - B[i*n + j];

    return C;
}

/*  ONLY FOR SMALL MATRICES
    maybe 4x4 ?             */
double* mul(double* A, double* B, int n) {
    double* C = initMat(n, ZERO);
    printf("-- %s\n", __func__);

    int i,j,k;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            for (k = 0; k < n; k++)
                C[i*n + j] += A[i*n + k] * B[k*n + j];

    return C;
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
double* merge(double* C11, double* C12, double* C21, double* C22, int n) {
    int big_n = n*2;
    double* C = initMat(big_n, sizeof(*C));
    int i, j;

    //printf("\tStarting C combine!!\n");
    for (i = 0; i < big_n; i++) {
        for (j = 0; j < big_n; j++) {
            bool isLeft  = j < (big_n / 2),
                 isUpper = i < (big_n / 2);

            if (isUpper) {
                if (isLeft)
                    C[i*big_n + j] = C11[i*n + j];
                else
                    C[i*big_n + j] = C12[i*n + j%n];
            }
            else {
                if (isLeft)
                    C[i*big_n + j] = C21[(i%n)*n + j];
                else
                    C[i*big_n + j] = C22[(i%n)*n + j%n];
            }
        }
    }
    //printf("\tFinished C combine!!\n");

    return C;
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
double* strassen(double* A, double* B, int n) {
    printf("\n=====\nStart strassen with %d x %d!\n\n", n, n);
	//if (n % 16 == 0)
	//	printf("- ");
    double* C; // = malloc(n * n * sizeof(double));


    // Small enough to use normal multiplication
    if (n <= 4) {
        //C = mul(A, B, n);
        return mul(A, B, n);
    }


    // If not even, expand to even
    // 5x5 -> 6x6
    if (n % 2 != 0) {
        double* expA = expand(A, n);
        double* expB = expand(B, n);
        
        //free(A); free(B);
        A = expA; B = expB;

        //n = closestHighPow(n);
        n++;
    }

    // Matrices after splitting into 4
    double *A11, *A12, *A21, *A22;
    double *B11, *B12, *B21, *B22;
    double *C11, *C12, *C21, *C22;

    A11 = initMat(n / 2, sizeof(double));
    A12 = initMat(n / 2, sizeof(double));
    A21 = initMat(n / 2, sizeof(double));
    A22 = initMat(n / 2, sizeof(double));

    B11 = initMat(n / 2, sizeof(double));
    B12 = initMat(n / 2, sizeof(double));
    B21 = initMat(n / 2, sizeof(double));
    B22 = initMat(n / 2, sizeof(double));

    C11 = initMat(n / 2, sizeof(double));
    C12 = initMat(n / 2, sizeof(double));
    C21 = initMat(n / 2, sizeof(double));
    C22 = initMat(n / 2, sizeof(double));

    printf("Finish calloc for A/B/C 11...22!\n\n");

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

    /*  M1 = (A11 + A22) * (B11 + B22)
        M2 = (A21 + A22) * B11
        M3 = A11 * (B12 - B22)
        M4 = A22 * (B21 - B11)
        M5 = (A11 + A12) * B22
        M6 = (A21 - A11) * (B11 + B12)
        M7 = (A12 - A22) * (B21 + B22)  */

    double  *A11_add_A22 = add(A11, A22, n/2),
            *B11_add_B22 = add(B11, B22, n/2),
            *A21_add_A22 = add(A21, A22, n/2),
            *B12_sub_B22 = sub(B12, B22, n/2),
            *B21_sub_B11 = sub(B21, B11, n/2),
            *A11_add_A12 = add(A11, A12, n/2),
            *A21_sub_A11 = sub(A21, A11, n/2),
            *B11_add_B12 = add(B11, B12, n/2),
            *A12_sub_A22 = sub(A12, A22, n/2),
            *B21_add_B22 = add(B21, B22, n/2);

    // M1 = strassen(add(A11, A22, n/2), add(B11, B22, n/2), n/2);
    // M2 = strassen(add(A21, A22, n/2), B11               , n/2);
    // M3 = strassen(A11               , sub(B12, B22, n/2), n/2);
    // M4 = strassen(A22               , sub(B21, B11, n/2), n/2);
    // M5 = strassen(add(A11, A12, n/2), B22               , n/2);
    // M6 = strassen(sub(A21, A11, n/2), add(B11, B12, n/2), n/2);
    // M7 = strassen(sub(A12, A22, n/2), add(B21, B22, n/2), n/2);

    M1 = strassen(A11_add_A22, B11_add_B22, n/2);
    M2 = strassen(A21_add_A22, B11        , n/2);
    M3 = strassen(A11        , B12_sub_B22, n/2);
    M4 = strassen(A22        , B21_sub_B11, n/2);
    M5 = strassen(A11_add_A12, B22        , n/2);
    M6 = strassen(A21_sub_A11, B11_add_B12, n/2);
    M7 = strassen(A12_sub_A22, B21_add_B22, n/2);

    printf("\nFinished calloc for M1...7!\n\n");

    free(A11_add_A22); free(B11_add_B22);
    free(A21_add_A22); free(B12_sub_B22);
    free(B21_sub_B11); free(A11_add_A12);
    free(A21_sub_A11); free(B11_add_B12);
    free(A12_sub_A22); free(B21_add_B22);

    /*  C11 = (M1 + M4) - (M5 - M7)
        C12 = M3 + M5
        C21 = M2 + M4
        C22 = (M1 - M2) + (M3 + M6) */

    double  *M1_add_M4 = add(M1, M4, n/2),
            *M5_sub_M7 = sub(M5, M7, n/2),
            *M1_sub_M2 = sub(M1, M2, n/2),
            *M3_add_M6 = add(M3, M6, n/2);

    C11 = sub(M1_add_M4, M5_sub_M7, n/2);
    C12 = add(M3, M5, n/2);
    C21 = add(M2, M4, n/2);
    C22 = add(M1_sub_M2, M3_add_M6, n/2);

    free(M1); free(M2); free(M3); free(M4);
    free(M5); free(M6); free(M7);

    free(M1_add_M4); free(M5_sub_M7);
    free(M1_sub_M2); free(M3_add_M6);    

    // printf(" -- Finished M calc! Combining to form C matrix --------------\n\n");

    

    C = merge(C11, C12, C21, C22, n/2);

    // printf("--- Finished combining C! --------------\n\n");

    free(A11); free(A12); free(A21); free(A22);
    free(B11); free(B12); free(B21); free(B22);
    free(C11); free(C12); free(C21); free(C22);

    return C;

}

double* strassenMain(double* A, double* B, int n) {
    double* C;
    int expanded_n = (n % 2 == 1? n + 1 : n);

    C = strassen(A, B, n);

    // free(C);    // must free before trimming

    printMat(C, expanded_n);
    //printArr(C, expanded_n);

    return C;

    // double** C_out;

    // trim(C, C_out, n, expanded_n);

    // printMat(*C_out, n);

    // return C_out;
}

/*  =======================================================================
    ======================================================================= */


int main(int argc, char** argv) {

// ------------------------------------------
    // Declarations
    //int n = argv[1]? atoi(argv[1]) : 3, 
    int          n = 9,
        expanded_n = n;

    // Matrices
    double* A = initMat(n, ONE_RING); //calloc(n * n, sizeof(double));
    double* B = initMat(n, ONE_RING); //calloc(n * n, sizeof(double));
    double* C; // = malloc(n * n * sizeof(double));
    //A = initMat(A, n, ONE_RING);
    //B = initMat(B, n, ONE_RING);
    //C = initMat(C, n, ZERO);

    expanded_n = (n % 2 == 1? n + 1 : n);

    printf("\n\n\n");

// ---------------------------------------
clock_t startTime, endTime, totalTime;
startTime = clock();

        // --- Print only ----
        // printMat(A, expanded_n);
        // printMat(B, expanded_n);
        // printMat(C, expanded_n);

        // --- Other ops -----
        // C = add(A, B, n);
        // printMat(C, n);


        // --- Normal mul ----
        // C = mul(A, B, n);
        // C = trim(&C, n, expanded_n);
        // printMat(C, n);


        // --- Strassen mul ---
        C = strassenMain(A, B, n);          /* TODO: This has 39 callocs  */
        //C = trim(&C, n, expanded_n);

        // printf("-------------------------------------\n");
        // printf("--------- After Strassen: -----------\n");
        //printMat(C, n);



startTime = clock() - startTime;
totalTime = endTime - startTime;
printf("\nTotal time = %lf\n", (double)startTime / CLOCKS_PER_SEC);
// ----------------------------------------

    free(A); free(B); free(C);

}
