#include "algo.h"
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