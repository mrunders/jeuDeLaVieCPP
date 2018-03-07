#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "Const.h"

#ifndef __travailleur_h__
#define __travailleur_h__ 1

#define envoyerAMaster(valeur) MPI_Send(&valeur, 1, MPI_INT, 0, 0, MPI_COMM_WORLD)

class travailleur {

public:

    travailleur(int _id_rank): 
    id_rank(_id_rank)
    {
        
        MPI_Recv(&max_y,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Recv(&max_x,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        
        int tableau[max_x][max_y];
                
        MPI_Recv(tableau,max_x*max_y,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        matrice = (int**) calloc(max_x, sizeof(int*));
        for (int i=0;i<max_x;++i){
            matrice[i] = (int*) malloc(sizeof(int)*max_y);
            for (int j=0;j<max_y;++j) matrice[i][j] = tableau[i][j]+1;
        }
        
        if (OUI) std::cout << "posy:" << max_x << " posx:" << max_y  << " FINI " << id_rank << "\n";

        for (; ; ) travailler();
        
    };

    int getrankid() { return id_rank; };


private:

    int max_x;
    int max_y;
    int id_rank;
    int **matrice;

    void travailler(){

        int action;
        MPI_Sendrecv(&TRAVAILLEUR_FINI, 1, MPI_INT, 0, 0, &action, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (OUI) std::cout << "ack de master commancons\n";
        
        switch (action){

            case(TRAVAILEUR_DONNER_RESULTAT):
                for (int i=0;i<max_x;++i)
                    for (int j=0;j<max_y;++j){
                        if (OUI) std::cout << i << ":" << j << " ";
                        envoyerAMaster(i);
                        envoyerAMaster(j);
                        envoyerAMaster(matrice[i][j]);
                    }

                envoyerAMaster(TRAVAILEUR_TRANSACTION_FINI);
                break;
        }
    }

};

#endif