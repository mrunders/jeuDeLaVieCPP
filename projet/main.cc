#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define FATALE_ERREUR(s,n) fprintf(stderr, s),exit(n)

using std::vector;

typedef enum ETATS { RIEN = 0, NOUVELLE, VIVANTE, MORTE };

int TAILLE_MPI;
int RANK_MPI;
ETATS **tableau;

int main(int argc , char**argv){

    //if (argc != 2) FATALE_ERREUR("param 1  : nombre de worker\n", 126);
    if (argc != 4) FATALE_ERREUR("param 1, 2 : taille de la matrice (x,y)\n\
                                  param 3 : nombre de workers (mpi -n <i-1>\n", 126);

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&TAILLE_MPI);
    MPI_Comm_rank(MPI_COMM_WORLD,&RANK_MPI);

    //std::cout << "taille: " << TAILLE_MPI << " rank: " << RANK_MPI << "\n";

    if ( !RANK_MPI ){

        int posx = atoi(argv[1]), 
            posy = atoi(argv[2]), 
            posz = posy / (TAILLE_MPI-1),
            nbElementParWorkers = posx * posz;

        ETATS matriceDeBase[posx][posy];

        for (int id_rank_send = 1; id_rank_send < TAILLE_MPI; ++id_rank_send){
            MPI_Send( &posx,
                      1, MPI_INT, id_rank_send ,
                      0, MPI_COMM_WORLD);

            MPI_Send( &posz,
                      1, MPI_INT, id_rank_send ,
                      0, MPI_COMM_WORLD);

            MPI_Send( matriceDeBase+(id_rank_send*sizeof(int*)),
                      nbElementParWorkers, MPI_INT, id_rank_send ,
                      0, MPI_COMM_WORLD);
        
        }

    } else {

        int posx, posy;

        MPI_Recv(&posx,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&posy,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        ETATS tableau[posx][posy];
        
        MPI_Recv(tableau,posx*posy,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        std::cout << "posx:" << posx << " posy:" << posy  << " FINI " << RANK_MPI << "\n";
    }

    MPI_Finalize();

    exit(0);
}