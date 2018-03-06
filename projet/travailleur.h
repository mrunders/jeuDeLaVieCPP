#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#ifndef __travailleur_h__
#define __travailleur_h__ 1

class travailleur {

public:

    travailleur(int _id_rank): 
    id_rank(_id_rank)
    {
        
        MPI_Recv(&max_x,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&max_y,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        
        int tableau[max_x][max_y];
                
        MPI_Recv(tableau,max_x*max_y,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        matrice = (int**) calloc(max_x, sizeof(int*));
        for (int i=0;i<max_x;++i){
            matrice[i] = (int*) malloc(sizeof(int)*max_y);
            memcpy(tableau[i], matrice[i], sizeof(int)*max_y);
        }
        
        std::cout << "posx:" << max_x << " posy:" << max_y  << " FINI " << id_rank << "\n";
    };

    int getrankid() { return id_rank; };


private:

    int max_x;
    int  max_y;
    int id_rank;
    int   **matrice;

};

#endif