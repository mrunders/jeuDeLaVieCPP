

COPTION = -std=gnu++11 -Wall -Wextra -O3
LOPTION = -std=c++11 -lpthread
LOPTION_SFML = $(LOPTION) -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system


main : main.o 
	mpic++ -o projet main.o $(LOPTION_SFML)

main.o : main.cc
	mpic++ $(COPTION) -c main.cc 

clean:
	rm -rf *.o
	rm -rf projet

zip: clean
	zip lison_laurent.zip *
