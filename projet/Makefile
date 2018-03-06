

COPTION = -std=c++11 -Wall -Wextra -O3
LOPTION = -std=c++11 -lpthread
LOPTION_SFML = $(LOPTION) -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system


main : main.o
	mpic++ -o projet main.o $(LOPTION)

main.o : main.cc
	mpic++ $(COPTION) -c main.cc

fenetre.o : fenetre.cpp fenetre.h
	g++ $(COPTION_SFML) -c fenetre.cpp

clean:
	rm -rf *.o
	rm -rf projet

zip: clean
	zip lison_laurent.zip *
