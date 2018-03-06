#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "master.h"
#include "travailleur.h"

#define FATALE_ERREUR(s,n) fprintf(stderr, s),exit(n)

using std::vector;

int TAILLE_MPI;
int RANK_MPI;

int main(int argc , char**argv){

    if (argc != 4) FATALE_ERREUR("param 1, 2 : taille de la matrice (x,y)\n\
                                  param 3 : nombre de workers (mpi -n <i-1>\n", 126);

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&TAILLE_MPI);
    MPI_Comm_rank(MPI_COMM_WORLD,&RANK_MPI);

    if ( !RANK_MPI )
        master *m = new master(TAILLE_MPI, atoi(argv[1]), atoi(argv[2]));

    else 
        travailleur *t = new travailleur(RANK_MPI);


    MPI_Finalize();

    exit(0);
}