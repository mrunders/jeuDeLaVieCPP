#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>

#include "worker.h"

using std::vector;
int IN = 0;

void afficherVecteur( std::vector<int> tab){

    unsigned int i;

    putchar('[');
    for (i = 0; i < tab.size(); ++i)
        std::cout << tab[i] << ", ";

    putchar(']'); putchar('\n');
}

int main(int argc ,char **argv){

    if(argc != 4){
        std::cout  << "Parameter 1 : number of threads" << std::endl;
        std::cout  << "Parameter 2 : number " << std::endl;
	std::cout  << "Paramater 3 : size of table" << std::endl;
        return  0;
    }

    IN = atoi(argv[2]);
    
    Workers workers(atoi(argv[1]));
    std::vector<int> vec;

    workers.initialize(vec ,atoi(argv[3]));
	aff("pouet");
    workers.waitResult();
    afficherVecteur(vec);

    workers.maximum(vec);
    aff("pouet");
    workers.waitResult();
    std::cout << "maximum: " << workers.getResult() << std::endl;

    workers.nbOccurrences(vec, IN);
	aff("pouet");
    workers.waitResult();
    std::cout << "occurences de " << IN << ": " << workers.getResult() << std::endl;

    workers.firstOccurence(vec, IN);
	aff("pouet");
    workers.waitResult();
    std::cout << "premiere position de " << IN << ": " << workers.getResult() << std::endl;

    return  0;
    
}
