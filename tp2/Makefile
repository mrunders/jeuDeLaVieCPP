COPTIONS= -std=c++11 -O3 -Wall -Wextra  -lpthread

CC= -g -Wall -Wextra -lpthread

main : main.o worker.o
	g++ -o pi main.o worker.o $(CC)

main.o : main.cc
	g++ $(COPTIONS) -c main.cc

worker.o : worker.cpp worker.h
	g++ $(COPTIONS) -Wall -c worker.cpp

clean:
	rm -rf *.o
