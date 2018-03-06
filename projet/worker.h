#ifndef  WORKERS
#define  WORKERS

#include <stdlib.h>
#include <vector>
#include <mutex>
#include <thread>

#include "Barrier.h"

using std::vector;
using std::thread;
using std::mutex;

#define tache_initialise 1
#define tache_max        2
#define tache_nbocc      3
#define tache_premocc    4

class  Workers {

    public:
        Workers(int  _nbThreads);
        ~Workers();

        void  initialize(int size);
        void  maximum();
        void  nbOccurrences(int  occ);
        void  firstOccurence(int  occ);
        void  afficherVecteur();
        void  waitResult();

        inline int getResult() { return this->results; }
        inline int nombreAleatoire() {return rand() % 1000; }

    private:
        const int nbThreads;

        volatile bool threadsfonctionfini;
        volatile bool threadsjobcommance;
        volatile bool threadsreveil;
        volatile bool threadsmort;

        Barrier *barrier;

        int  objectif;
        int  results;
        int  idtache;

        std::vector<std::thread> threads;
        std::vector<int> tab;

        std::condition_variable threadssyncreveil;
        std::condition_variable threadssyncdeb;
        std::condition_variable threadssyncfin;

        std::mutex threadssyncreveilm;
        std::mutex threadssyncdebm;
        std::mutex threadsfonctionfinim;

        std::mutex results_mutex;
        std::mutex idtache_mutex;

        void executetache(int idt);
        void remplirTab(int idt);
        void calculeDuMax(int idt);
        void trouverOccurances(int idt);
        void premsOccurence(int idt);
};

#endif
