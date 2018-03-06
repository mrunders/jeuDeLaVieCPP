#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#ifndef __master_h__
#define __master_h__ 1

class master {

public:

    master(int tmpi, int _mx, int _my): 
    max_x(_mx), max_y(_my), id_rank(0)
    {
        
        int posz = _my / (tmpi-1),
            nbElementParWorkers = _mx * posz;

        int matriceDeBase[_mx][_my];
        for (int i=0;i<_mx;++i) memset(matriceDeBase[i], 0, sizeof(int)*_my);

        for (int id_rank_send = 1; id_rank_send < tmpi; ++id_rank_send){
            MPI_Send( &_mx,
                  1, MPI_INT, id_rank_send ,
                  0, MPI_COMM_WORLD);

            MPI_Send( &posz,
                  1, MPI_INT, id_rank_send ,
                  0, MPI_COMM_WORLD);

            MPI_Send( matriceDeBase+(id_rank_send*sizeof(int*)),
                  nbElementParWorkers, MPI_INT, id_rank_send ,
                  0, MPI_COMM_WORLD);
        }
    };

    int getrankid() { return id_rank; };

private:

    int max_x;
    int  max_y;
    int id_rank;
    int   **matrice;

};

#endif