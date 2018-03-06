#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <string.h>

using std::vector;

int main(int argc , char**argv){

    srand(time(NULL));

    int size, rank;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int *tab;
    int taille, pingPong = 0;
    const int limit=100;
    int partnerRank=(rank+1)%2;

    for ( ; pingPong < limit; ){
        if (rank == pingPong%2 ){
            pingPong++;

            taille = rand()%20;
           /* tab = (int*) calloc(taille, sizeof(int));
            memset(tab, 1, sizeof(int)*taille);
                */
            MPI_Send(&taille,1,MPI_INT,partnerRank,0,MPI_COMM_WORLD);
           // MPI_Send(tab, taille, MPI_INT, partnerRank, 0, MPI_COMM_WORLD);
           
    std::cout << "0" << std::endl;

        } else {
          

            MPI_Recv(&taille, 1, MPI_INT, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            /*tab = (int*) calloc(taille, sizeof(int));
            MPI_Recv(tab, taille, MPI_INT, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            std::cout << taille << "[" ;
            for (int i=0; i < taille; ++i) std::cout << tab[i] << ",";
            std::cout << "]" << std::endl;*/
            
    std::cout << "1" << std::endl;
        }
    }
    MPI_Finalize();
}
