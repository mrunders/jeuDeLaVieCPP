COPTION = -std=c++11 -Wall -Wextra -O3
LOPTION = -lpthread

main : main.o
	mpic++ -o hello main.o $(LOPTION)
	
main.o : main.cc
	mpic++ $(COPTION) -c main.cc