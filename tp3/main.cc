#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <string.h>
#include <unistd.h>

using std::vector;

int main(int argc , char**argv){

    if (argc != 2){

        std::cout << "param 1  : number" << std::endl;
        return 127;
    }

    srand(time(NULL));

    int size, rank;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int *tab;
    int taille=0;
    const int limit= atoi(argv[1]);
    int partnerRank=(rank+1)%2;

    for ( ; taille < limit ; ){

        if(rank == taille%2){

            taille++;

            taille = 1 + rand()%20;
            tab = (int*) calloc(taille, sizeof(int));
            memset(tab, 1, sizeof(int)*taille);

            MPI_Send(&taille,1,MPI_INT,partnerRank,0,MPI_COMM_WORLD);

            sleep(1);

            MPI_Send(tab, taille, MPI_INT, partnerRank, 0, MPI_COMM_WORLD);

            sleep(1);

        } else {

            MPI_Recv(&taille,1,MPI_INT,partnerRank,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            tab = (int*) calloc(taille, sizeof(int));

            MPI_Recv(tab, taille, MPI_INT, partnerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            std::cout << taille << "[" ;
            for (int i=0; i < taille; ++i) std::cout << tab[i] << ",";
            std::cout << "]" << std::endl;

        }
    }
    MPI_Finalize();
}
