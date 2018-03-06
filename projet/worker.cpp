#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <condition_variable>
#include "worker.h"
#include "Barrier.h"
#include <unistd.h>

#define setvar_mutex(var, varmutex, choix) varmutex.lock();\
var = choix;\
varmutex.unlock()

using std::vector;
using std::condition_variable;
using std::unique_lock;

Workers::Workers(int  _nbThreads ):
    nbThreads(_nbThreads),
    threadsfonctionfini(false),
    threadsjobcommance(false),
    threadsreveil(false),
    threadsmort(false),
    barrier(new Barrier(nbThreads)),
    objectif(0),
    results(0),
    idtache(0)

{

    for (int i = 0; i < nbThreads; ++i)
        threads.push_back(std::thread(&Workers::executetache, this, i));
    
}

Workers::~Workers(){

    threadsmort = true;
    threadsjobcommance = true;
    threadssyncdeb.notify_all();

    for (int i = 0; i < nbThreads; ++i)
        threads[i].join();

    
    free(barrier);
}

void Workers::executetache(int idt){

    int id;

    for ( ; ; ){

        // condition variable pour reveiller les threads
        std::unique_lock<std::mutex> lk(threadssyncdebm);
        threadssyncdeb.wait(lk, [this]{return threadsjobcommance; });
        lk.unlock();

        // si on reveil les threads via le destructeur
        if ( threadsmort ) return;

        // synchronication des threads
        barrier->wait();
        threadsjobcommance = false;

        // choix de la fonction a executer
        idtache_mutex.lock();
        id = this->idtache;
        idtache_mutex.unlock();

        switch (id){
            case(tache_initialise):
                remplirTab(idt);
                break;

            case(tache_max):
                calculeDuMax(idt);
                break;

            case(tache_nbocc):
                trouverOccurances(idt);
                break;

            case(tache_premocc):
                premsOccurence(idt);
                break;  
        }

        //sleep( 1 + idt>>1 );
        // synchronication des threads une fois le travail fini
        if ( barrier->wait() ) threadsfonctionfini = true;

    }
}


void Workers::afficherVecteur(){
    
    unsigned int i;
    
    putchar('[');
    for (i = 0; i < tab.size(); ++i)
        std::cout << tab[i] << ", ";
    
    putchar(']'); putchar('\n');
}

//  Initialise  le  tableau  en le  remplissant  de  nombre  aleatoire
void Workers::remplirTab(int idt){

    unsigned int taille = tab.size() / this->nbThreads;
    unsigned int indice = taille * idt;

    if (idt == this->nbThreads -1)
        for ( ; indice < tab.size(); ++indice) tab[indice] = nombreAleatoire();

    else 
        for ( ; taille; --taille) tab[indice++] = nombreAleatoire();

    
}

void Workers::initialize(int size){

    for (int i = 0; i < size; ++i) tab.push_back(0);

    setvar_mutex(idtache, idtache_mutex, tache_initialise);

    {
        std::lock_guard<std::mutex> lk(threadssyncdebm);
        threadsjobcommance = true;
    }

    threadssyncdeb.notify_all();

}

//  Trouve  le  maximum  du  tableau
void Workers::calculeDuMax(int idt){

    int max = 0;
    unsigned int taille = tab.size() / this->nbThreads;
    unsigned int indice = taille * idt;

    if (idt == this->nbThreads -1){

        for ( ; indice < tab.size(); ++indice) 
            if ( max < tab[indice]) max = tab[indice];

    } else {

        for ( ; taille; --taille, ++indice)
            if ( max < tab[indice]) max = tab[indice];

    }

    this->results_mutex.lock();
    if ( this->results < max ) this->results = max;
    this->results_mutex.unlock();
}

void  Workers::maximum(){

    if ( tab.size() == 0){
        std::cout << " erreur: tableau non initialiser (abandon)\n";
        return;
    }

    setvar_mutex(idtache, idtache_mutex, tache_max);
    setvar_mutex(results, results_mutex, 0);

    {
        std::lock_guard<std::mutex> lk(threadssyncdebm);
        threadsjobcommance = true;
    }
    
    threadssyncdeb.notify_all();

}

//  Trouve  le  nombre d’occurrences d’une  valeur
void Workers::trouverOccurances(int idt){

    int nb = 0;
    unsigned int taille = tab.size() / this->nbThreads;
    unsigned int indice = taille * idt;

    if (idt == this->nbThreads -1){

        for ( ; indice < tab.size(); ++indice) 
            if ( objectif == tab[indice]) ++nb;

    } else {

        for ( ; taille; --taille, ++indice)
            if ( objectif ==  tab[indice]) ++nb;

    }

    setvar_mutex(results, results_mutex, results + nb);

}

void  Workers::nbOccurrences(int  occ){


    if ( tab.size() == 0){
        std::cout << " erreur: tableau non initialiser (abandon)\n";
        return;
    }

    objectif = occ;
    setvar_mutex(idtache, idtache_mutex, tache_nbocc);
    setvar_mutex(results, results_mutex, 0);

    {
        std::lock_guard<std::mutex> lk(threadssyncdebm);
        threadsjobcommance = true;
    }
    
    threadssyncdeb.notify_all();
}

//  Trouve  la  position  de la  premiere  occurrence d’une  valeur
void Workers::premsOccurence(int idt){

    unsigned int taille = tab.size() / this->nbThreads;
    unsigned int indice = taille * idt;

    if (idt == this->nbThreads -1){

        for ( ; indice < tab.size(); ++indice) 
            if ( objectif == tab[indice]) break;

    } else {

        for ( ; taille; --taille, ++indice)
            if ( objectif ==  tab[indice]) break;

    }


    this->results_mutex.lock();
    if (this->results == -1 && indice < tab.size() && tab[indice] == objectif) this->results = indice;
    this->results_mutex.unlock();
}

void  Workers::firstOccurence(int  occ){

    if ( tab.size() == 0){
        std::cout << " erreur: tableau non initialiser (abandon)\n";
        return;
    }

    objectif = occ;
    setvar_mutex(idtache, idtache_mutex, tache_premocc);
    setvar_mutex(results, results_mutex, -1);

    {
        std::lock_guard<std::mutex> lk(threadssyncdebm);
        threadsjobcommance = true;
    }
    
    threadssyncdeb.notify_all();
}


void Workers::waitResult(){

    for ( ; !threadsfonctionfini ; );

    threadsjobcommance = false;
    threadsreveil = false;
    threadsfonctionfini = false;

}
