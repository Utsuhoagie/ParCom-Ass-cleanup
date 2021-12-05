
#include <stdio.h>
#define N 10000 //N lan luot la 100, 1000, 10000
int main( int argc, char* argv[] ){
    FILE *myfile;
    myfile = fopen("./matrix10000.txt", "w+");
    
    for (int i = 0; i<N; i++){
        for(int j = 0; j < N; j++)
            fputs("1 ", myfile);
        fputs("\n", myfile);
    }
    fclose(myfile);
    return 0;
}