#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include "Const.h"
#include "fenetre.h"

#ifndef __master_h__
#define __master_h__ 1

class master {

public:

    master(int tmpi, int _mx, int _my): 
    max_x(_mx), max_y(_my), id_rank(0), nbTravailleurs(tmpi-1), fen(new fenetre(_mx,_my))
    {
        
        int posz = _mx / nbTravailleurs,
            nbElementParWorkers = _my * posz;

        int matriceDeBase[_mx][_my];
        for (int i=0;i<_mx;++i) for (int j=0;j<_my;++j) matriceDeBase[i][j] = 0;

        if (OUI) std::cout << "split en (" << posz << "," << _mx << ")" << nbElementParWorkers << "\n";

        for (int id_rank_send = 1; id_rank_send < tmpi; ++id_rank_send){

            MPI_Send( &_mx,
                  1, MPI_INT, id_rank_send ,
                  0, MPI_COMM_WORLD);

            MPI_Send( &posz,
                  1, MPI_INT, id_rank_send ,
                  0, MPI_COMM_WORLD);

            MPI_Send( matriceDeBase+(id_rank_send-1)*posz,
                  nbElementParWorkers, MPI_INT, id_rank_send ,
                  0, MPI_COMM_WORLD);

        }

        std::thread f(&master::lancerInterface, this);
        
        ecoute();

        f.join();
    };

    ~master(){

        for( int i=0; i < max_x; ++i) free(matrice[i]);
        free(matrice);
        free(fen);
    }


    int getrankid() { return id_rank; };

private:

    int max_x;
    int  max_y;
    int id_rank;
    int nbTravailleurs;
    int   **matrice;
    fenetre *fen;

    void lancerInterface() { fen->boucle(); };

    void ecoute() {

        int messagereceived = 0,flag = 0, nbTravailleurFini;
        MPI_Status status;
        MPI_Request request;

        nbTravailleurFini = 0;
        for(;;) {
            flag = 0;
            MPI_Irecv(&messagereceived, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            while(!flag) {
                MPI_Test(&request, &flag, &status);
                if(flag) {
                    switch(messagereceived) {
                        case (TRAVAILLEUR_FINI): ++nbTravailleurFini; break;
                        case (98): break;
                        case (97): return;
                        default:
                            printf("recived %d from [worker d] problem \n", messagereceived);
                            exit(0);
                    }
                    break;
                }
            }

            if (nbTravailleurFini == nbTravailleurs){

                nbTravailleurFini = 0;

                int x,y,v;
                int info;
                MPI_Status status;

                if (OUI) std::cout << "tout les travailleurs ont fini leur premiere itération\n";
                
                for (int id_rank_send = 1; id_rank_send < nbTravailleurs+1; ++id_rank_send){

                    if (OUI) std::cout << "demande d'info de " << id_rank_send << "\n";

                    MPI_Send( &TRAVAILEUR_DONNER_RESULTAT,
                        1, MPI_INT, id_rank_send,
                        0, MPI_COMM_WORLD);

                    if (OUI) std::cout << "on commance avec " << id_rank_send << "\n";

                    for ( ; ; ){

                        MPI_Recv(&x,1,MPI_INT,id_rank_send,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

                        if (x == TRAVAILEUR_TRANSACTION_FINI) break;

                        MPI_Recv(&y,1,MPI_INT,id_rank_send,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        MPI_Recv(&v,1,MPI_INT,id_rank_send,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

                        if (OUI) std::cout << "RECU DE " << id_rank_send << " (" << (id_rank_send-1)*x << "," << y << "," << v << ")\n";

                        fen->changerPixel((id_rank_send-1)*x,y,v);
                    }

                    if (OUI) std::cout << "transaction fini avec " << id_rank_send << "\n";
                }

                if (OUI) std::cout << "TOUT EST FINI\n";
                fen->maj();

                std::cout << "turn\n";
            }
        }
    } 

};

#endif