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

#define INITIALISER_RESULTAT(i) this->mutext.lock();\
this->results = i;\
this->mutext.unlock();

#define aff(s) std::cout << s << std::endl;

#define cvf() threadsJobFinished = false

#define INITIALISE_TAB 1
#define TROUVER_MAX    2
#define NB_OCCURANCES  3
#define PREMIER_OCC    4

class  Workers {

    public:
        Workers(int  _nbThreads, int _tailleTableau);
        ~Workers ();

        void  initialize(int idt);
        void  maximum(std::vector<int>& tab);
        void  nbOccurrences(std::vector<int>& tab , int  occ);
        void  firstOccurence(std::vector<int>& tab , int  occ);
        void  waitResult();

        inline int getResult() { return this->results; }
        inline int nombreAleatoire() {return rand() % 1000; }

    private:
        const  int  nbThreads;

        std::vector<std::thread*> threads;
        std::vector<int> tab;

        Barrier *barrier;

        int  results;

        std::mutex mutext;
        volatile bool threadsJobFinished;

        std::mutex idTacheMutex;
        int idTache;

        void calculeDuMax(std::vector<int>& tab, int idt);
        void trouverOccurances(std::vector<int>& tab, int idt, int valeur);
        void premsOccurence(std::vector<int>& tab, int idt, int valeur);
};

#endif
