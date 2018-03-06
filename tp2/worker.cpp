#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "worker.h"
#include "Barrier.h"

using std::vector;

Workers::Workers(int  _nbThreads, int _tailleTableau):
    nbThreads(_nbThreads),
    results(0),
    barrier(new Barrier(nbThreads)),
    threadsJobFinished(true)

{

    for (int i=0; i < _tailleTableau; ++i) tab.push_back(0);

    for (int i = 0; i < nbThreads; ++i) threads.push_back(new std::thread(executerTache));
}

Workers::~Workers(){

    free(barrier);

    for (int i = 0; i < nbThreads; ++i) free(threads[i]);
}

//  Initialise  le  tableau  en le  remplissant  de  nombre  aleatoire
void Workers::initialize(int idt){

    unsigned int taille = tab.size() / this->nbThreads;
    unsigned int indice = taille * idt;

    if (idt == this->nbThreads -1){

        for ( ; indice < tab.size(); ++indice) tab[indice] = nombreAleatoire();

    } else {

        for ( ; taille; --taille) tab[indice++] = nombreAleatoire();

    }

    if (barrier->wait()) threadsJobFinished = true;
}

//  Trouve  le  maximum  du  tableau
void Workers::maximum(int idt){

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

    this->mutext.lock();
    if ( this->results < max ) this->results = max;
    this->mutext.unlock();

    if (barrier->wait()) threadsJobFinished = true;
}

//  Trouve  le  nombre d’occurrences d’une  valeur
void Workers::trouverOccurances(std::vector<int>& tab, int idt, int valeur){

    int nb = 0;
    unsigned int taille = tab.size() / this->nbThreads;
    unsigned int indice = taille * idt;

    if (idt == this->nbThreads -1){

        for ( ; indice < tab.size(); ++indice) 
            if ( valeur == tab[indice]) ++nb;

    } else {

        for ( ; taille; --taille, ++indice)
            if ( valeur ==  tab[indice]) ++nb;

    }

    this->mutext.lock();
    this->results += nb;
    this->mutext.unlock();

    if (barrier->wait()) threadsJobFinished = true;
}

void  Workers::nbOccurrences(std:: vector<int>& tab , int  occ){

    int i;
	cvf();
    INITIALISER_RESULTAT(0);

    if (tab.size() == 0 ){
        std::cout << "erreur tableau vide" << std::endl;
        exit(127);
    }

    for (i = 0; i < this->nbThreads; ++i)
        threads[i] = new std::thread(&Workers::trouverOccurances, this, std::ref(tab), i, occ);

}

//  Trouve  la  position  de la  premiere  occurrence d’une  valeur
void Workers::premsOccurence(std::vector<int>& tab, int idt, int valeur){

    unsigned int taille = tab.size() / this->nbThreads;
    unsigned int indice = taille * idt;

    if (idt == this->nbThreads -1){

        for ( ; indice < tab.size(); ++indice) 
            if ( valeur == tab[indice]) break;

    } else {

        for ( ; taille; --taille, ++indice)
            if ( valeur ==  tab[indice]) break;

    }


    this->mutext.lock();
    if (this->results == -1) this->results = indice;
    this->mutext.unlock();

    if (barrier->wait()) threadsJobFinished = true;
}

void  Workers::firstOccurence(std:: vector<int>& tab , int  occ){

    int i;
	cvf();
    INITIALISER_RESULTAT(-1);

    if (tab.size() == 0 ){
        std::cout << "erreur tableau vide" << std::endl;
        exit(127);
    }

	
    for (i = 0; i < this->nbThreads; ++i)
        threads[i] = new std::thread(&Workers::premsOccurence, this, std::ref(tab), i, occ);
}


void Workers::executerTache(idt){

    idTacheMutex.lock();
    int tmp = idTache;
    idTacheMutex.unlock();

    switch ( tmp ){
        case( INITIALISE_TAB ):
            initialize(idt);
            break;
    }
}

void Workers::waitResult(){

    for ( ; !threadsJobFinished ; );

    std::cout << "tous les thread ont fini" << std::endl;
}


/*

std::thread(executer);

fonction startInit() --> mutex travail = 0 ;  executer()
fonction startMax()  --> mutex travail = 1 ;  executer() 

executer(){
    if (travai == 0) remplirtab()
        taivail== 1  max()
}





*/